/*
    musicmove - Bulk renamer for music files
    Copyright (C) 2016-2017  Adam Szmigin (adam.szmigin@xsco.net)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "config.hpp"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <algorithm>
#include <iostream>
#include <vector>
#include <stdexcept>

#include "context.hpp"
#include "move.hpp"

namespace po = boost::program_options;
namespace fs = boost::filesystem;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;
using std::vector;
using std::for_each;
using std::exception;

static void print_usage(std::ostream &os, const po::options_description &od)
{
    os << PACKAGE_STRING " - Bulk renamer for music files"
       << endl
       << endl
       << "Usage: " << PACKAGE " --format=str path1 [path2..]"
       << endl
       << endl
       << od
       << endl
       << "Report bugs to: " PACKAGE_BUGREPORT
       << endl
       << endl
       << PACKAGE " home page: <http://musicmove.xsco.org>"
       << endl;
}

static void print_version(std::ostream &os)
{
    os << PACKAGE_STRING " - Bulk renamer for music files"
       << endl
       << endl
       << "Copyright (C) 2017 Adam Szmigin (adam.szmigin@xsco.net)"
       << endl
       << "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>"
       << endl
       << "This is free software: you are free to change and redistribute it."
       << endl
       << "There is NO WARRANTY, to the extent permitted by law."
       << endl;
}

static void print_ex_and_abort()
{
    auto e = std::current_exception();
    if (e != nullptr)
    {
        try { std::rethrow_exception(e); }
        catch (std::exception &e) { cerr << e.what() << endl; }
    }
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    std::set_terminate(print_ex_and_abort);

    // Command-line options visible in the help screen
    po::options_description od_visible("Allowed options");
    od_visible.add_options()
        ("format", po::value<string>(),
            "The file path format string.  The syntax is a file path, but "
            "certain tokens are replaced with values from each file's "
            "metadata.  `%a' for artist, `%b' for album, `%c' for comment, "
            "`%d' for disc number, `%e' for encoded by, `%g' for genre, `%l' "
            "for track total, `%n' for track number (zero-padded to two "
            "characters), `%o' for original artist, `%p' for composer, `%r' "
            "for copyright, `%t' for title, `%u' for URL, `%x' for disc "
            "total, `%y' for year, `%z' for album artist (falling back to "
            "artist if not set), or `%%' for a literal percent sign.")
        ("format-script", po::value<string>(),
            "Path to a file that contains ChaiScript code which can be used "
            "to move/rename files in different ways.  Constant variables are "
            "provided for path, parent_dir, filename, and filename_stem.  Tags "
            "may be accessed via the functions artist(), album(), comment(), "
            "disc_number(), encoded_by(), genre(), track_total(), "
            "track_number(), original_artist(), composer(), copyright(), "
            "title(), url(), disc_total(), year(), and album_artist().  The "
            "ChaiScript code must return the new file path to work.  If the "
            "code returns tokens like `%a' etc, then they will be expanded as "
            "usual.")
        ("simulate,s", po::bool_switch(),
            "Simulate renaming, i.e. don't commit any changes to disk. "
            "This is the default.")
        ("for-real,f", po::bool_switch(),
            "Rename files for real; " PACKAGE " is safe by default, so you "
            "will need to use this switch to explicitly tell the program that "
            "you actually want it to rename files.")
        ("path-conversion", po::value<string>(),
            "Approach for converting meta-data characters into paths.\n"
            "`windows-ascii' converts to 7-bit ASCII and removes any "
            "characters not normally permitted on Microsoft Windows machines.\n"
            "`utf-8' uses the full expressivity of UTF-8 Unicode paths, "
            "assuming they are supported by your filesystem.\n"
            "`posix' is a restrictive but portable approach that permits only "
            "unaccented letters (of either case), numbers, dot, underscore, "
            "and hyphen.\n"
            "The default option is `" PATH_CONVERSION_DEFAULT_VALUE "'.\n")
        ("exit-on-duplicate", po::bool_switch(),
            "Exit if we encounter two files that would be rewritten to the "
            "same path on disk (default is to skip any such duplicates).")
        ("verbose,v", po::bool_switch(),
            "Print additional messages about what's going on.")
        ("version", po::bool_switch(),
            "Display the version number and exit.")
        ("help,h", po::bool_switch(),
            "Display this help screen.")
        ;

    // Options that are not shown in a help screen
    po::options_description od_hidden("Hidden options");
    od_hidden.add_options()
        ("path", po::value<vector<string> >(),
            "input path to files for renaming")
        ;

    // Set the path as a positional parameter
    po::positional_options_description pos;
    pos.add("path", -1);

    // Parse command-line options    
    po::options_description od;
    od.add(od_visible).add(od_hidden);
    po::variables_map vm;
    po::parsed_options parsed = po::command_line_parser(argc, argv)
        .options(od)
        .positional(pos)
        .run();
    po::store(parsed, vm);
    po::notify(vm);
    

    // Cry for help?
    if (vm["help"].as<bool>())
    {
        print_usage(cout, od_visible);
        return 0;
    }
    
    if (vm["version"].as<bool>())
    {
        print_version(cout);
        return 0;
    }
    
    // Do we have at least one path specified?
    if (vm.count("path") <= 0)
    {
        cerr << "No path(s) specified" << endl;
        cerr << "Run `" PACKAGE " --help' for information on usage" << endl;
        return 1;
    }
    
    // Do we have a format specified?
    if (vm.count("format") <= 0 && vm.count("format-script") <= 0)
    {
        cerr << "No format string or script specified" << endl;
        cerr << "Run `" PACKAGE " --help' for information on usage" << endl;
        return 1;
    }
    
    // Simulate and for-real both specified?
    if (vm["for-real"].as<bool>() && vm["simulate"].as<bool>())
    {
        cerr << "The `simulate' and `for-real' options cannot both be "
             << "specified at the same time: please choose just one" << endl;
        cerr << "Run `" PACKAGE " --help' for information on usage" << endl;
        return 1;
    }
    
    // Form context struct
    mm::context ctx;
    if (vm.count("format-script") > 0)
    {
        ctx.use_format_script = true;
        ctx.format_script = vm["format-script"].as<string>();
    }
    else
    {
        ctx.use_format_script = false;
        ctx.format = vm["format"].as<string>();
    }
    ctx.simulate = !vm["for-real"].as<bool>();
    ctx.verbose = vm["verbose"].as<bool>();
    ctx.path_uniqueness = vm.count("exit-on-duplicate") <= 0
        ? mm::path_uniqueness_t::skip
        : mm::path_uniqueness_t::exit;
    auto path_conversion_str = vm.count("path-conversion") <= 0
        ? PATH_CONVERSION_DEFAULT_VALUE
        : vm["path-conversion"].as<string>();
    if (path_conversion_str == "windows-ascii")
        ctx.path_conversion = mm::path_conversion_t::windows_ascii;
    else if (path_conversion_str == "utf-8")
        ctx.path_conversion = mm::path_conversion_t::utf8;
    else if (path_conversion_str == "posix")
        ctx.path_conversion = mm::path_conversion_t::posix;
    else
    {
        cerr << "Unknown path-conversion value `" << path_conversion_str << "'"
             << endl;
        return 1;
    }

    // Process specified paths
    const vector<string> &paths = vm["path"].as<vector<string>>();
    for (auto &path_str : paths)
    {
        fs::path p{path_str};
        try
        {
            mm::process_path(p, ctx);
        }
        catch (std::exception &e)
        {
            cerr << e.what() << endl;
            return 1;
        }
    }
    
    return 0;
}


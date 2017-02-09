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
#include <config.h>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <fileref.h>
#include <tpropertymap.h>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

namespace po = boost::program_options;
namespace fs = boost::filesystem;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;
using std::vector;
using std::for_each;

static void print_usage(std::ostream &os, const po::options_description &od)
{
    os << "Usage:"
       << endl
       << PACKAGE " command --format=str [--base-dir=path] path1 [path2..]"
       << endl
       << od
       << endl;
}

static void process_file(const fs::path &p, const po::variables_map &vm)
{
    TagLib::FileRef f{p.c_str()};
    
    // Does this file have any audio properties?
    if (f.audioProperties() == nullptr)
    {
        cerr << "No audio properties found for file "
             << p << ".. skipping" << endl;
        return;
    }
    
    // Does this file have a tag?
    auto *tag = f.tag();
    if (tag == nullptr)
    {
        cerr << "No tag found for file " << p << ".. skipping" << endl;
        return;
    }
    
    auto props = tag->properties();
    stringstream ss;
    
    // FIXME temp
    if (props.isEmpty())
    {
        ss << "(none)";
    }
    else
    {
        auto begin = props.begin();
        ss << begin->first;
        for_each(++begin, props.end(),
            [&ss](auto &kve) { ss << ", " << kve.first; });
    }
    
    // TODO - check filename and directory name validity, according to Boost:
    // www.boost.org/doc/libs/1_58_0/libs/filesystem/doc/portability_guide.htm
    
    // TODO - convert weird characters in filenames
    
    cout << "File: " << p << ", tags: " << ss.str() << endl;
}

static void process_path(const fs::path &p, const po::variables_map &vm)
{
    if (!exists(p))
    {
        cerr << "Warning: path does not exist: " << p << endl;
        return;
    }

    // Regular file or dir?
    if (fs::is_directory(p))
    {
        // It's a directory.  Recurse over everything within.
        for_each(fs::directory_iterator{p},
                 fs::directory_iterator{},
                 [&vm](auto &de) { process_path(de.path(), vm); });
        
        // Is the directory now empty?
        // TODO - remove the directory if it is now empty
    }
    else
    {
        // Read as a file.
        process_file(p, vm);
    }
};

int main(int argc, char *argv[])
{
    // Command-line options visible in the help screen
    po::options_description od_visible("Allowed options");
    od_visible.add_options()
        ("format,f", po::value<string>(),
            "file path format string")
        ("base-dir,b", po::value<string>()->default_value("."),
            "base directory from which relative paths will start")
        ("simulate,s", po::bool_switch(),
            "whether to simulate or rename for real")
        ("verbose,v", po::bool_switch(),
            "print additional messages about what's going on")
        ("help,h", po::bool_switch(),
            "display this help screen")
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
    
    // Do we have at least one path specified?
    if (vm.count("path") <= 0)
    {
        print_usage(cout, od_visible);
        return 1;
    }
    
    // Do we have a format specified?
    if (vm.count("format") <= 0)
    {
        cerr << "No format string specified" << endl << endl;
        print_usage(cout, od_visible);
        return 1;
    }

    // Process specified paths
    const vector<string> &paths = vm["path"].as<vector<string>>();
    for (auto &path_str : paths)
    {
        fs::path p{path_str};
        process_path(p, vm);
    }
    
    return 0;
}


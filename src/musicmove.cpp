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

#include <algorithm>
#include <iostream>
#include <vector>

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

static void print_usage(std::ostream &os, const po::options_description &od)
{
    os << "Usage:"
       << endl
       << PACKAGE " command --format=str [--base-dir=path] path1 [path2..]"
       << endl
       << od
       << endl;
}

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
        ("very-verbose", po::bool_switch(),
            "print even more messages about what's going on")
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

    // Form context struct
    mm::context ctx;
    ctx.format = vm["format"].as<string>();
    ctx.base_dir = fs::path{vm["base-dir"].as<string>()};
    ctx.simulate = vm["simulate"].as<bool>();
    ctx.verbose = ctx.very_verbose = false;
    ctx.verbose = vm["verbose"].as<bool>();
    ctx.very_verbose = vm["very-verbose"].as<bool>();
    if (ctx.very_verbose)
        ctx.verbose = true;

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


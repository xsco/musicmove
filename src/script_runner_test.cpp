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
#include "script_runner.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE script_runner_test
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <fstream>
#include <string>

#define STRINGIFY(x) STRINGIFY_(x)
#define STRINGIFY_(x) #x

namespace fs = boost::filesystem;
using namespace std;

const string testdata_dir_str = STRINGIFY(TESTDATA_DIR) "/script_runner";


BOOST_AUTO_TEST_CASE (chaiscript_format)
{
    mm::context ctx;
    ctx.use_format_script = true;
    fs::path file{testdata_dir_str + "/foo.txt"};
    BOOST_CHECK(fs::exists(file));
    mm::metadata tag{file};
    
    // Try out simple script.
    ctx.format_script = fs::path{testdata_dir_str + "/01_staticvalue.chai"};
    BOOST_CHECK(fs::exists(ctx.format_script));
    BOOST_CHECK_EQUAL(
        mm::get_format_from_script(file, tag, ctx),
        "foobarbaz");
    
    // Script referencing built-in variables.
    ctx.format_script = fs::path{testdata_dir_str + "/02_constvars.chai"};
    BOOST_CHECK(fs::exists(ctx.format_script));
    BOOST_CHECK_EQUAL(
        mm::get_format_from_script(file, tag, ctx),
        "foo.txt");
    
    // Script referencing tokens.
    ctx.format_script = fs::path{testdata_dir_str + "/03_tokens.chai"};
    BOOST_CHECK(fs::exists(ctx.format_script));
    BOOST_CHECK_EQUAL(
        mm::get_format_from_script(file, tag, ctx),
        "%g/%z/%b/%d%n-%a-%t_%%");
    
    // Script referencing tag functions.
    ctx.format_script = fs::path{testdata_dir_str + "/04_tags.chai"};
    BOOST_CHECK(fs::exists(ctx.format_script));
    BOOST_CHECK_EQUAL(
        mm::get_format_from_script(file, tag, ctx),
        "genre/albumartist/album/discnumbertracknumber-artist-title");
}

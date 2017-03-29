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
#include "move.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE move_test
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <stdexcept>

#define STRINGIFY(x) STRINGIFY_(x)
#define STRINGIFY_(x) #x

namespace fs = boost::filesystem;
using namespace std;

const string move_testdata_dir_str = STRINGIFY(TESTDATA_DIR) "/move";

struct fixture
{
    fixture() :
        tmp_dir{fs::temp_directory_path() /
                fs::path{"musicmove-" + fs::unique_path().string()}}
    {
        // Ensure the tmp dir exists
        cout << "Creating " << tmp_dir << endl;
        if (fs::exists(tmp_dir))
            throw std::runtime_error{"tmp_dir already exists!"};
        fs::create_directories(tmp_dir);
    }
    
    ~fixture()
    {
        // Remove the tmp dir if it exists
        cout << "Removing " << tmp_dir << endl;
        if (fs::is_directory(tmp_dir))
            fs::remove_all(tmp_dir);
    }
    
    const fs::path tmp_dir;
};

BOOST_AUTO_TEST_CASE (rename_same_dir)
{
    fixture f;
    
    // Move a file within the same directory
    cout << "Temp dir for files: " << f.tmp_dir << endl;
    // TODO - test renaming a file within the same dir
}

BOOST_AUTO_TEST_CASE (move_new_dir)
{
    fixture f;
    
    // TODO - test moving a file to a new dir
}

BOOST_AUTO_TEST_CASE (move_new_dir_clash)
{
    fixture f;
    
    // TODO - test moving a file to a new dir with a name clash
}


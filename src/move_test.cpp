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

const fs::path sample_file{STRINGIFY(TESTDATA_DIR) "/move/foo.txt"};

struct fixture
{
    fixture() :
        tmp_dir{fs::temp_directory_path() /
                fs::path{"musicmove-" + fs::unique_path().string()}}
    {
        // Ensure the tmp dir exists
        cout << "Creating " << tmp_dir << endl;
        if (fs::exists(tmp_dir))
        {
            throw std::runtime_error{"tmp_dir already exists!"};
        }
        fs::create_directories(tmp_dir);
    }
    
    ~fixture()
    {
        // Remove the tmp dir if it exists
        cout << "Removing " << tmp_dir << endl;
        if (fs::is_directory(tmp_dir))
        {
            fs::remove_all(tmp_dir);
        }
    }
    
    const fs::path tmp_dir;
};

BOOST_AUTO_TEST_CASE (rename_file_same_dir)
{
    fixture f;
    
    mm::context ctx;
    ctx.format = f.tmp_dir.string();
    ctx.simulate = false;
    ctx.verbose = true;
    ctx.path_uniqueness = mm::path_uniqueness_t::exit;
    ctx.path_conversion = mm::path_conversion_t::posix;
    
    // Set up files for test
    fs::path s1{f.tmp_dir / "001a.inc"};
    fs::path s2{f.tmp_dir / "002b.inc"};
    fs::path s3{f.tmp_dir / "003c.inc"};
    fs::path s4{f.tmp_dir / "004d.inc"};
    fs::path d1{f.tmp_dir / "101-AA1-TT1.inc"};
    fs::path d2{f.tmp_dir / "102-AA1-TT2.inc"};
    fs::path d3{f.tmp_dir / "201-AA1-TT3.inc"};
    fs::path d4{f.tmp_dir / "202-AA1-TT4.inc"};
    fs::copy_file(sample_file, s1);
    fs::copy_file(sample_file, s2);
    fs::copy_file(sample_file, s3);
    fs::copy_file(sample_file, s4);
    
    // Move a file within the same directory
    auto results1 = mm::move_file(s1, ctx);
    auto results2 = mm::move_file(s2, ctx);
    auto results3 = mm::move_file(s3, ctx);
    auto results4 = mm::move_file(s4, ctx);
    
    // Check results
    BOOST_CHECK_EQUAL(results1.filename_changed, true);
    BOOST_CHECK_EQUAL(results1.dir_changed, false);
    BOOST_CHECK_EQUAL(results1.moved_out_of_parent_dir, false);
    BOOST_CHECK_EQUAL(results2.filename_changed, true);
    BOOST_CHECK_EQUAL(results2.dir_changed, false);
    BOOST_CHECK_EQUAL(results2.moved_out_of_parent_dir, false);
    BOOST_CHECK_EQUAL(results3.filename_changed, true);
    BOOST_CHECK_EQUAL(results3.dir_changed, false);
    BOOST_CHECK_EQUAL(results3.moved_out_of_parent_dir, false);
    BOOST_CHECK_EQUAL(results4.filename_changed, true);
    BOOST_CHECK_EQUAL(results4.dir_changed, false);
    BOOST_CHECK_EQUAL(results4.moved_out_of_parent_dir, false);
    
    // Check situation on disk
    BOOST_CHECK_EQUAL(fs::exists(s1), false);
    BOOST_CHECK_EQUAL(fs::exists(s2), false);
    BOOST_CHECK_EQUAL(fs::exists(s3), false);
    BOOST_CHECK_EQUAL(fs::exists(s4), false);
    BOOST_CHECK_EQUAL(fs::exists(d1), true);
    BOOST_CHECK_EQUAL(fs::exists(d2), true);
    BOOST_CHECK_EQUAL(fs::exists(d3), true);
    BOOST_CHECK_EQUAL(fs::exists(d4), true);
}

BOOST_AUTO_TEST_CASE (rename_file_same_dir_clash)
{
    fixture f;
    
    mm::context ctx;
    ctx.format = f.tmp_dir.string();
    ctx.simulate = false;
    ctx.verbose = true;
    ctx.path_uniqueness = mm::path_uniqueness_t::exit;
    ctx.path_conversion = mm::path_conversion_t::posix;
    
    fs::path s1{f.tmp_dir / "001a.inc"};
    fs::path s2{f.tmp_dir / "001b.inc"};
    fs::path d1{f.tmp_dir / "101-AA1-TT1.inc"};
    fs::copy_file(sample_file, s1);
    fs::copy_file(sample_file, s2);
    
    // Move a file within the same directory
    auto results1 = mm::move_file(s1, ctx);
    BOOST_CHECK_EQUAL(results1.filename_changed, true);
    BOOST_CHECK_EQUAL(results1.dir_changed, false);
    BOOST_CHECK_EQUAL(results1.moved_out_of_parent_dir, false);
    BOOST_CHECK_EQUAL(fs::exists(s1), false);
    BOOST_CHECK_EQUAL(fs::exists(d1), true);
    
    // Second call should throw with a uniqueness problem
    BOOST_CHECK_THROW(mm::move_file(s2, ctx), mm::path_uniqueness_violation);
    BOOST_CHECK_EQUAL(fs::exists(s2), true); // Should still exist
    BOOST_CHECK_EQUAL(fs::exists(d1), true); // Should still exist
    
    // Try again, but opting to skip when encountering a uniqueness problem
    ctx.path_uniqueness = mm::path_uniqueness_t::skip;
    auto results2 = mm::move_file(s2, ctx);
    BOOST_CHECK_EQUAL(results2.filename_changed, false);
    BOOST_CHECK_EQUAL(results2.dir_changed, false);
    BOOST_CHECK_EQUAL(results2.moved_out_of_parent_dir, false);
    BOOST_CHECK_EQUAL(fs::exists(s2), true); // Should still exist
    BOOST_CHECK_EQUAL(fs::exists(d1), true); // Should still exist
    
}

BOOST_AUTO_TEST_CASE (move_file_new_dir_diff_hier_sideways)
{
    fixture f;

    mm::context ctx;
    ctx.format = f.tmp_dir.string();
    ctx.simulate = false;
    ctx.verbose = true;
    ctx.path_uniqueness = mm::path_uniqueness_t::exit;
    ctx.path_conversion = mm::path_conversion_t::posix;
    
    // Set up two directories in the tmp area
    fs::path hier1{f.tmp_dir / "Alb1"};
    fs::path hier2{f.tmp_dir / "Alb2"};
    fs::create_directory(hier1);
    
    // Set up files for test
    fs::path s1{hier1 / "009a.inc"};
    fs::path s2{hier1 / "010b.inc"};
    fs::path s3{hier1 / "011c.inc"};
    fs::path s4{hier1 / "012d.inc"};
    fs::path d1{hier2 / "101-AA1-TT1.inc"};
    fs::path d2{hier2 / "102-AA1-TT2.inc"};
    fs::path d3{hier2 / "201-AA1-TT3.inc"};
    fs::path d4{hier2 / "202-AA1-TT4.inc"};
    fs::copy_file(sample_file, s1);
    fs::copy_file(sample_file, s2);
    fs::copy_file(sample_file, s3);
    fs::copy_file(sample_file, s4);
    
    // Move files to a new dir in a different hierarchy, "sideways"
    auto results1 = mm::move_file(s1, ctx);
    auto results2 = mm::move_file(s2, ctx);
    auto results3 = mm::move_file(s3, ctx);
    auto results4 = mm::move_file(s4, ctx);
    
    // Check results
    BOOST_CHECK_EQUAL(results1.filename_changed, true);
    BOOST_CHECK_EQUAL(results1.dir_changed, true);
    BOOST_CHECK_EQUAL(results1.moved_out_of_parent_dir, true);
    BOOST_CHECK_EQUAL(results2.filename_changed, true);
    BOOST_CHECK_EQUAL(results2.dir_changed, true);
    BOOST_CHECK_EQUAL(results2.moved_out_of_parent_dir, true);
    BOOST_CHECK_EQUAL(results3.filename_changed, true);
    BOOST_CHECK_EQUAL(results3.dir_changed, true);
    BOOST_CHECK_EQUAL(results3.moved_out_of_parent_dir, true);
    BOOST_CHECK_EQUAL(results4.filename_changed, true);
    BOOST_CHECK_EQUAL(results4.dir_changed, true);
    BOOST_CHECK_EQUAL(results4.moved_out_of_parent_dir, true);
    
    // Check situation on disk
    BOOST_CHECK_EQUAL(fs::exists(s1), false);
    BOOST_CHECK_EQUAL(fs::exists(s2), false);
    BOOST_CHECK_EQUAL(fs::exists(s3), false);
    BOOST_CHECK_EQUAL(fs::exists(s4), false);
    BOOST_CHECK_EQUAL(fs::exists(d1), true);
    BOOST_CHECK_EQUAL(fs::exists(d2), true);
    BOOST_CHECK_EQUAL(fs::exists(d3), true);
    BOOST_CHECK_EQUAL(fs::exists(d4), true);
}

BOOST_AUTO_TEST_CASE (move_file_new_dir_diff_hier_upwards)
{
    fixture f;

    mm::context ctx;
    ctx.format = f.tmp_dir.string();
    ctx.simulate = false;
    ctx.verbose = true;
    ctx.path_uniqueness = mm::path_uniqueness_t::exit;
    ctx.path_conversion = mm::path_conversion_t::posix;
    
    // Set up two directories in the tmp area
    fs::path hier1{f.tmp_dir / "Alb1"};
    fs::path start_dir{hier1 / "foo"};
    fs::create_directory(hier1);
    fs::create_directory(start_dir);
    
    // Set up files for test
    fs::path s1{start_dir / "005a.inc"};
    fs::path s2{start_dir / "006b.inc"};
    fs::path s3{start_dir / "007c.inc"};
    fs::path s4{start_dir / "008d.inc"};
    fs::path d1{hier1 / "101-AA1-TT1.inc"};
    fs::path d2{hier1 / "102-AA1-TT2.inc"};
    fs::path d3{hier1 / "201-AA1-TT3.inc"};
    fs::path d4{hier1 / "202-AA1-TT4.inc"};
    fs::copy_file(sample_file, s1);
    fs::copy_file(sample_file, s2);
    fs::copy_file(sample_file, s3);
    fs::copy_file(sample_file, s4);
    
    // Move files to a new dir in a different hierarchy, "upwards"
    auto results1 = mm::move_file(s1, ctx);
    auto results2 = mm::move_file(s2, ctx);
    auto results3 = mm::move_file(s3, ctx);
    auto results4 = mm::move_file(s4, ctx);
    
    // Check results
    BOOST_CHECK_EQUAL(results1.filename_changed, true);
    BOOST_CHECK_EQUAL(results1.dir_changed, true);
    BOOST_CHECK_EQUAL(results1.moved_out_of_parent_dir, true);
    BOOST_CHECK_EQUAL(results2.filename_changed, true);
    BOOST_CHECK_EQUAL(results2.dir_changed, true);
    BOOST_CHECK_EQUAL(results2.moved_out_of_parent_dir, true);
    BOOST_CHECK_EQUAL(results3.filename_changed, true);
    BOOST_CHECK_EQUAL(results3.dir_changed, true);
    BOOST_CHECK_EQUAL(results3.moved_out_of_parent_dir, true);
    BOOST_CHECK_EQUAL(results4.filename_changed, true);
    BOOST_CHECK_EQUAL(results4.dir_changed, true);
    BOOST_CHECK_EQUAL(results4.moved_out_of_parent_dir, true);
    
    // Check situation on disk
    BOOST_CHECK_EQUAL(fs::exists(s1), false);
    BOOST_CHECK_EQUAL(fs::exists(s2), false);
    BOOST_CHECK_EQUAL(fs::exists(s3), false);
    BOOST_CHECK_EQUAL(fs::exists(s4), false);
    BOOST_CHECK_EQUAL(fs::exists(d1), true);
    BOOST_CHECK_EQUAL(fs::exists(d2), true);
    BOOST_CHECK_EQUAL(fs::exists(d3), true);
    BOOST_CHECK_EQUAL(fs::exists(d4), true);
}

BOOST_AUTO_TEST_CASE (move_file_new_dir_diff_hier_clash)
{
    fixture f;

    mm::context ctx;
    ctx.format = f.tmp_dir.string();
    ctx.simulate = false;
    ctx.verbose = true;
    ctx.path_uniqueness = mm::path_uniqueness_t::exit;
    ctx.path_conversion = mm::path_conversion_t::posix;
    
    // Set up two directories in the tmp area
    fs::path hier1{f.tmp_dir / "Alb1"};
    fs::path hier2{f.tmp_dir / "Alb2"};
    fs::create_directory(hier1);
    
    // Set up files for test
    fs::path s1{hier1 / "009a.inc"};
    fs::path s2{hier1 / "009b.inc"};
    fs::path d1{hier2 / "101-AA1-TT1.inc"};
    fs::copy_file(sample_file, s1);
    fs::copy_file(sample_file, s2);
    
    // Move files to a new dir in a different hierarchy with a name clash
    auto results1 = mm::move_file(s1, ctx);
    BOOST_CHECK_EQUAL(results1.filename_changed, true);
    BOOST_CHECK_EQUAL(results1.dir_changed, true);
    BOOST_CHECK_EQUAL(results1.moved_out_of_parent_dir, true);
    BOOST_CHECK_EQUAL(fs::exists(s1), false);
    BOOST_CHECK_EQUAL(fs::exists(d1), true);
    
    // Attempt to move 2nd file should throw with a uniqueness problem
    BOOST_CHECK_THROW(mm::move_file(s2, ctx), mm::path_uniqueness_violation);
    BOOST_CHECK_EQUAL(fs::exists(s2), true); // Should still exist
    BOOST_CHECK_EQUAL(fs::exists(d1), true); // Should still exist
    
    // Attempt to move 2nd file, but skip on uniqueness violation
    ctx.path_uniqueness = mm::path_uniqueness_t::skip;
    auto results2 = mm::move_file(s2, ctx);
    BOOST_CHECK_EQUAL(results2.filename_changed, false);
    BOOST_CHECK_EQUAL(results2.dir_changed, false);
    BOOST_CHECK_EQUAL(results2.moved_out_of_parent_dir, false);
    BOOST_CHECK_EQUAL(fs::exists(s2), true); // Should still exist
    BOOST_CHECK_EQUAL(fs::exists(d1), true); // Should still exist
}

BOOST_AUTO_TEST_CASE (move_file_new_dir_same_hier)
{
    fixture f;

    mm::context ctx;
    ctx.format = f.tmp_dir.string();
    ctx.simulate = false;
    ctx.verbose = true;
    ctx.path_uniqueness = mm::path_uniqueness_t::exit;
    ctx.path_conversion = mm::path_conversion_t::posix;
    
    // Set up directory in the tmp area
    fs::path hier1{f.tmp_dir / "Alb1"};
    fs::create_directory(hier1);
    
    // Set up files for test
    fs::path s1{f.tmp_dir / "005a.inc"};
    fs::path s2{f.tmp_dir / "006b.inc"};
    fs::path s3{f.tmp_dir / "007c.inc"};
    fs::path s4{f.tmp_dir / "008d.inc"};
    fs::path d1{hier1 / "101-AA1-TT1.inc"};
    fs::path d2{hier1 / "102-AA1-TT2.inc"};
    fs::path d3{hier1 / "201-AA1-TT3.inc"};
    fs::path d4{hier1 / "202-AA1-TT4.inc"};
    fs::copy_file(sample_file, s1);
    fs::copy_file(sample_file, s2);
    fs::copy_file(sample_file, s3);
    fs::copy_file(sample_file, s4);
    
    // Move files to a new dir within the same hierarchy
    auto results1 = mm::move_file(s1, ctx);
    auto results2 = mm::move_file(s2, ctx);
    auto results3 = mm::move_file(s3, ctx);
    auto results4 = mm::move_file(s4, ctx);
    
    // Check results
    BOOST_CHECK_EQUAL(results1.filename_changed, true);
    BOOST_CHECK_EQUAL(results1.dir_changed, true);
    BOOST_CHECK_EQUAL(results1.moved_out_of_parent_dir, false);
    BOOST_CHECK_EQUAL(results2.filename_changed, true);
    BOOST_CHECK_EQUAL(results2.dir_changed, true);
    BOOST_CHECK_EQUAL(results2.moved_out_of_parent_dir, false);
    BOOST_CHECK_EQUAL(results3.filename_changed, true);
    BOOST_CHECK_EQUAL(results3.dir_changed, true);
    BOOST_CHECK_EQUAL(results3.moved_out_of_parent_dir, false);
    BOOST_CHECK_EQUAL(results4.filename_changed, true);
    BOOST_CHECK_EQUAL(results4.dir_changed, true);
    BOOST_CHECK_EQUAL(results4.moved_out_of_parent_dir, false);
    
    // Check situation on disk
    BOOST_CHECK_EQUAL(fs::exists(s1), false);
    BOOST_CHECK_EQUAL(fs::exists(s2), false);
    BOOST_CHECK_EQUAL(fs::exists(s3), false);
    BOOST_CHECK_EQUAL(fs::exists(s4), false);
    BOOST_CHECK_EQUAL(fs::exists(d1), true);
    BOOST_CHECK_EQUAL(fs::exists(d2), true);
    BOOST_CHECK_EQUAL(fs::exists(d3), true);
    BOOST_CHECK_EQUAL(fs::exists(d4), true);
}

BOOST_AUTO_TEST_CASE (move_file_not_music)
{
    fixture f;

    mm::context ctx;
    ctx.format = f.tmp_dir.string();
    ctx.simulate = false;
    ctx.verbose = true;
    ctx.path_uniqueness = mm::path_uniqueness_t::exit;
    ctx.path_conversion = mm::path_conversion_t::posix;
    
    // Set up directory in the tmp area
    fs::path hier1{f.tmp_dir / "Alb1"};
    fs::create_directory(hier1);
    
    // Set up files for test
    fs::path s1{f.tmp_dir / "001a.exc"};
    fs::copy_file(sample_file, s1);
    
    // Attempt to rename a non-music file
    auto results1 = mm::move_file(s1, ctx);
    
    // Check results
    BOOST_CHECK_EQUAL(results1.filename_changed, false);
    BOOST_CHECK_EQUAL(results1.dir_changed, false);
    BOOST_CHECK_EQUAL(results1.moved_out_of_parent_dir, false);
}

BOOST_AUTO_TEST_CASE (process_path_flat_rename)
{
    fixture f;
    
    mm::context ctx;
    ctx.format = f.tmp_dir.string();
    ctx.simulate = false;
    ctx.verbose = true;
    ctx.path_uniqueness = mm::path_uniqueness_t::exit;
    ctx.path_conversion = mm::path_conversion_t::posix;
    
    // Set up dir for renaming files within
    fs::path hier1{f.tmp_dir / "Alb1"};
    fs::create_directory(hier1);
    
    // Set up files
    fs::path s1{hier1 / "005a.inc"};
    fs::path s2{hier1 / "006b.inc"};
    fs::path s3{hier1 / "007c.inc"};
    fs::path s4{hier1 / "008d.inc"};
    fs::path d1{hier1 / "101-AA1-TT1.inc"};
    fs::path d2{hier1 / "102-AA1-TT2.inc"};
    fs::path d3{hier1 / "201-AA1-TT3.inc"};
    fs::path d4{hier1 / "202-AA1-TT4.inc"};
    fs::copy_file(sample_file, s1);
    fs::copy_file(sample_file, s2);
    fs::copy_file(sample_file, s3);
    fs::copy_file(sample_file, s4);
    
    // Process the path
    auto results = mm::process_path(hier1, ctx);
    
    // Check results
    BOOST_CHECK_EQUAL(results.files_processed, 4);
    BOOST_CHECK_EQUAL(results.dirs_processed, 1);
    BOOST_CHECK_EQUAL(results.moved_out_of_parent_dir, false);
    
    // Check situation on disk
    BOOST_CHECK_EQUAL(fs::exists(s1), false);
    BOOST_CHECK_EQUAL(fs::exists(s2), false);
    BOOST_CHECK_EQUAL(fs::exists(s3), false);
    BOOST_CHECK_EQUAL(fs::exists(s4), false);
    BOOST_CHECK_EQUAL(fs::exists(d1), true);
    BOOST_CHECK_EQUAL(fs::exists(d2), true);
    BOOST_CHECK_EQUAL(fs::exists(d3), true);
    BOOST_CHECK_EQUAL(fs::exists(d4), true);
}

BOOST_AUTO_TEST_CASE (process_path_flat_move_out)
{
    fixture f;
    
    mm::context ctx;
    ctx.format = f.tmp_dir.string();
    ctx.simulate = false;
    ctx.verbose = true;
    ctx.path_uniqueness = mm::path_uniqueness_t::exit;
    ctx.path_conversion = mm::path_conversion_t::posix;
    
    // Set up dir for renaming files within
    fs::path hier1{f.tmp_dir / "Alb1"};
    fs::path start_dir{f.tmp_dir / "foo"};
    fs::create_directory(start_dir);
    
    // Set up files
    fs::path s1{start_dir / "005a.inc"};
    fs::path s2{start_dir / "006b.inc"};
    fs::path s3{start_dir / "007c.inc"};
    fs::path s4{start_dir / "008d.inc"};
    fs::path d1{hier1 / "101-AA1-TT1.inc"};
    fs::path d2{hier1 / "102-AA1-TT2.inc"};
    fs::path d3{hier1 / "201-AA1-TT3.inc"};
    fs::path d4{hier1 / "202-AA1-TT4.inc"};
    fs::copy_file(sample_file, s1);
    fs::copy_file(sample_file, s2);
    fs::copy_file(sample_file, s3);
    fs::copy_file(sample_file, s4);
    
    // Process the path
    auto results = mm::process_path(start_dir, ctx);
    
    // Check results
    BOOST_CHECK_EQUAL(results.files_processed, 4);
    BOOST_CHECK_EQUAL(results.dirs_processed, 1);
    BOOST_CHECK_EQUAL(results.moved_out_of_parent_dir, true);
    
    // Check situation on disk
    // Empty start_dir should have been removed
    BOOST_CHECK_EQUAL(fs::exists(start_dir), false);
    BOOST_CHECK_EQUAL(fs::exists(d1), true);
    BOOST_CHECK_EQUAL(fs::exists(d2), true);
    BOOST_CHECK_EQUAL(fs::exists(d3), true);
    BOOST_CHECK_EQUAL(fs::exists(d4), true);
}

BOOST_AUTO_TEST_CASE (process_path_nested_move_out_sideways)
{
    fixture f;
    
    mm::context ctx;
    ctx.format = f.tmp_dir.string();
    ctx.simulate = false;
    ctx.verbose = true;
    ctx.path_uniqueness = mm::path_uniqueness_t::exit;
    ctx.path_conversion = mm::path_conversion_t::posix;
    
    // Set up two directories in the tmp area
    fs::path hier1{f.tmp_dir / "Alb1"};
    fs::path hier2{f.tmp_dir / "Alb2"};
    fs::create_directory(hier1);
    
    // Set up files for test
    fs::path s1{hier1 / "009a.inc"};
    fs::path s2{hier1 / "010b.inc"};
    fs::path s3{hier1 / "011c.inc"};
    fs::path s4{hier1 / "012d.inc"};
    fs::path d1{hier2 / "101-AA1-TT1.inc"};
    fs::path d2{hier2 / "102-AA1-TT2.inc"};
    fs::path d3{hier2 / "201-AA1-TT3.inc"};
    fs::path d4{hier2 / "202-AA1-TT4.inc"};
    fs::copy_file(sample_file, s1);
    fs::copy_file(sample_file, s2);
    fs::copy_file(sample_file, s3);
    fs::copy_file(sample_file, s4);
    
    // Process the base path, expecting all files to move from Alb1 to Alb2
    auto results = mm::process_path(f.tmp_dir, ctx);
    
    // Check results
    BOOST_CHECK_EQUAL(results.files_processed, 4);
    BOOST_CHECK_EQUAL(results.dirs_processed, 2);
    BOOST_CHECK_EQUAL(results.moved_out_of_parent_dir, false);
    
    // Check situation on disk
    // Empty hier1 dir should have been removed
    BOOST_CHECK_EQUAL(fs::exists(hier1), false);
    BOOST_CHECK_EQUAL(fs::exists(hier2), true);
    BOOST_CHECK_EQUAL(fs::exists(d1), true);
    BOOST_CHECK_EQUAL(fs::exists(d2), true);
    BOOST_CHECK_EQUAL(fs::exists(d3), true);
    BOOST_CHECK_EQUAL(fs::exists(d4), true);
}

// TODO - add test cases in simulate mode

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
#include "metadata.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE metadata_test
#include <boost/test/unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <iostream>
#include <string>

#define STRINGIFY(x) STRINGIFY_(x)
#define STRINGIFY_(x) #x

namespace fs = boost::filesystem;
using namespace std;

const string file_formats_dir_str = STRINGIFY(TESTDATA_DIR) "/file_formats";

BOOST_AUTO_TEST_CASE (blank_metadata)
{
    // TODO - test files with no metadata, making sure we get empty strings
}

BOOST_AUTO_TEST_CASE (easytag_metadata)
{
    fs::path flac_path{file_formats_dir_str + "/01-chirp.easytag.flac"};
    fs::path mp4_path {file_formats_dir_str + "/02-chirp.easytag.m4a"};

    cout << "Testing " << flac_path << endl;
    BOOST_CHECK(fs::exists(flac_path));
    
    mm::metadata tag_flac{flac_path};
    BOOST_CHECK_EQUAL(tag_flac.album(),           "Album field");
    BOOST_CHECK_EQUAL(tag_flac.album_artist(),    "Album artist field");
    BOOST_CHECK_EQUAL(tag_flac.artist(),          "Artist field");
    BOOST_CHECK_EQUAL(tag_flac.comment(),         "Comment field");
    BOOST_CHECK_EQUAL(tag_flac.composer(),        "Composer field");
    BOOST_CHECK_EQUAL(tag_flac.copyright(),       "Copyright field");
    BOOST_CHECK_EQUAL(tag_flac.encoded_by(),      "Encoded by field");
    BOOST_CHECK_EQUAL(tag_flac.date(),            "2017");
    BOOST_CHECK_EQUAL(tag_flac.disc_number(),     "1");
    BOOST_CHECK_EQUAL(tag_flac.disc_total(),      "2");
    BOOST_CHECK_EQUAL(tag_flac.genre(),           "Pop");
    BOOST_CHECK_EQUAL(tag_flac.original_artist(), "Original artist field");
    BOOST_CHECK_EQUAL(tag_flac.title(),           "Title field");
    BOOST_CHECK_EQUAL(tag_flac.track_number(),    "1");
    BOOST_CHECK_EQUAL(tag_flac.track_total(),     "4");
    BOOST_CHECK_EQUAL(tag_flac.url(),             "URL field");
    
    cout << "Testing " << mp4_path << endl;
    BOOST_CHECK(fs::exists(mp4_path));
    
    mm::metadata tag_mp4{mp4_path};
    BOOST_CHECK_EQUAL(tag_mp4.album(),           "Album field");
    BOOST_CHECK_EQUAL(tag_mp4.album_artist(),    "Album artist field");
    BOOST_CHECK_EQUAL(tag_mp4.artist(),          "Artist field");
    BOOST_CHECK_EQUAL(tag_mp4.comment(),         "Comment field");
    BOOST_CHECK_EQUAL(tag_mp4.composer(),        "Composer field");
    BOOST_CHECK_EQUAL(tag_mp4.copyright(),       "Copyright field");
    BOOST_CHECK_EQUAL(tag_mp4.encoded_by(),      "Encoded by field");
    BOOST_CHECK_EQUAL(tag_mp4.date(),            "2017");
    BOOST_CHECK_EQUAL(tag_mp4.disc_number(),     "1");
    BOOST_CHECK_EQUAL(tag_mp4.disc_total(),      "2");
    BOOST_CHECK_EQUAL(tag_mp4.genre(),           "Pop");
    BOOST_CHECK_EQUAL(tag_mp4.original_artist(), ""); // No field available
    BOOST_CHECK_EQUAL(tag_mp4.title(),           "Title field");
    BOOST_CHECK_EQUAL(tag_mp4.track_number(),    "2");
    BOOST_CHECK_EQUAL(tag_mp4.track_total(),     "4");
    BOOST_CHECK_EQUAL(tag_mp4.url(),             ""); // No field available
}

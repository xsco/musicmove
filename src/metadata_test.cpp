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

// Check files minmally-tagged by EasyTag.
BOOST_AUTO_TEST_CASE (easytag_min_metadata)
{
    fs::path flac_path  {file_formats_dir_str + "/chirp.min.easytag.01.flac"};
    fs::path mp4_path   {file_formats_dir_str + "/chirp.min.easytag.02.m4a"};
    fs::path mpeg_path  {file_formats_dir_str + "/chirp.min.easytag.03.mp3"};
    fs::path vorbis_path{file_formats_dir_str + "/chirp.min.easytag.04.ogg"};

    cout << "Testing " << flac_path << endl;
    BOOST_CHECK(fs::exists(flac_path));
    
    mm::metadata tag_flac{flac_path};
    tag_flac.print_properties(cout);
    cout << endl;
    BOOST_CHECK_EQUAL(tag_flac.album(),           "Album field");
    BOOST_CHECK_EQUAL(tag_flac.album_artist(),    "");
    BOOST_CHECK_EQUAL(tag_flac.artist(),          "Artist field");
    BOOST_CHECK_EQUAL(tag_flac.comment(),         "");
    BOOST_CHECK_EQUAL(tag_flac.composer(),        "");
    BOOST_CHECK_EQUAL(tag_flac.copyright(),       "");
    BOOST_CHECK_EQUAL(tag_flac.encoded_by(),      "");
    BOOST_CHECK_EQUAL(tag_flac.date(),            "2017");
    BOOST_CHECK_EQUAL(tag_flac.disc_number(),     "");
    BOOST_CHECK_EQUAL(tag_flac.disc_total(),      "");
    BOOST_CHECK_EQUAL(tag_flac.genre(),           "Pop");
    BOOST_CHECK_EQUAL(tag_flac.original_artist(), "");
    BOOST_CHECK_EQUAL(tag_flac.title(),           "Title field");
    BOOST_CHECK_EQUAL(tag_flac.track_number(),    "1");
    BOOST_CHECK_EQUAL(tag_flac.track_total(),     "");
    BOOST_CHECK_EQUAL(tag_flac.url(),             "");
    
    cout << "Testing " << mp4_path << endl;
    BOOST_CHECK(fs::exists(mp4_path));
    
    mm::metadata tag_mp4{mp4_path};
    tag_mp4.print_properties(cout);
    cout << endl;
    BOOST_CHECK_EQUAL(tag_mp4.album(),           "Album field");
    BOOST_CHECK_EQUAL(tag_mp4.album_artist(),    "");
    BOOST_CHECK_EQUAL(tag_mp4.artist(),          "Artist field");
    BOOST_CHECK_EQUAL(tag_mp4.comment(),         "");
    BOOST_CHECK_EQUAL(tag_mp4.composer(),        "");
    BOOST_CHECK_EQUAL(tag_mp4.copyright(),       "");
    BOOST_CHECK_EQUAL(tag_mp4.encoded_by(),      "");
    BOOST_CHECK_EQUAL(tag_mp4.date(),            "2017");
    BOOST_CHECK_EQUAL(tag_mp4.disc_number(),     "");
    BOOST_CHECK_EQUAL(tag_mp4.disc_total(),      "");
    BOOST_CHECK_EQUAL(tag_mp4.genre(),           "Pop");
    BOOST_CHECK_EQUAL(tag_mp4.original_artist(), "");
    BOOST_CHECK_EQUAL(tag_mp4.title(),           "Title field");
    BOOST_CHECK_EQUAL(tag_mp4.track_number(),    "2");
    BOOST_CHECK_EQUAL(tag_mp4.track_total(),     "");
    BOOST_CHECK_EQUAL(tag_mp4.url(),             "");
    
    cout << "Testing " << mpeg_path << endl;
    BOOST_CHECK(fs::exists(mpeg_path));
    
    mm::metadata tag_mpeg{mpeg_path};
    tag_mpeg.print_properties(cout);
    cout << endl;
    BOOST_CHECK_EQUAL(tag_mpeg.album(),           "Album field");
    BOOST_CHECK_EQUAL(tag_mpeg.album_artist(),    "");
    BOOST_CHECK_EQUAL(tag_mpeg.artist(),          "Artist field");
    BOOST_CHECK_EQUAL(tag_mpeg.comment(),         "");
    BOOST_CHECK_EQUAL(tag_mpeg.composer(),        "");
    BOOST_CHECK_EQUAL(tag_mpeg.copyright(),       "");
    BOOST_CHECK_EQUAL(tag_mpeg.encoded_by(),      "");
    BOOST_CHECK_EQUAL(tag_mpeg.date(),            "2017");
    BOOST_CHECK_EQUAL(tag_mpeg.disc_number(),     "");
    BOOST_CHECK_EQUAL(tag_mpeg.disc_total(),      "");
    BOOST_CHECK_EQUAL(tag_mpeg.genre(),           "Pop");
    BOOST_CHECK_EQUAL(tag_mpeg.original_artist(), "");
    BOOST_CHECK_EQUAL(tag_mpeg.title(),           "Title field");
    BOOST_CHECK_EQUAL(tag_mpeg.track_number(),    "3");
    BOOST_CHECK_EQUAL(tag_mpeg.track_total(),     "");
    BOOST_CHECK_EQUAL(tag_mpeg.url(),             "");
    
    cout << "Testing " << vorbis_path << endl;
    BOOST_CHECK(fs::exists(vorbis_path));
    
    mm::metadata tag_vorbis{vorbis_path};
    tag_vorbis.print_properties(cout);
    cout << endl;
    BOOST_CHECK_EQUAL(tag_vorbis.album(),           "Album field");
    BOOST_CHECK_EQUAL(tag_vorbis.album_artist(),    "");
    BOOST_CHECK_EQUAL(tag_vorbis.artist(),          "Artist field");
    BOOST_CHECK_EQUAL(tag_vorbis.comment(),         "");
    BOOST_CHECK_EQUAL(tag_vorbis.composer(),        "");
    BOOST_CHECK_EQUAL(tag_vorbis.copyright(),       "");
    BOOST_CHECK_EQUAL(tag_vorbis.encoded_by(),      "");
    BOOST_CHECK_EQUAL(tag_vorbis.date(),            "2017");
    BOOST_CHECK_EQUAL(tag_vorbis.disc_number(),     "");
    BOOST_CHECK_EQUAL(tag_vorbis.disc_total(),      "");
    BOOST_CHECK_EQUAL(tag_vorbis.genre(),           "Pop");
    BOOST_CHECK_EQUAL(tag_vorbis.original_artist(), "");
    BOOST_CHECK_EQUAL(tag_vorbis.title(),           "Title field");
    BOOST_CHECK_EQUAL(tag_vorbis.track_number(),    "4");
    BOOST_CHECK_EQUAL(tag_vorbis.track_total(),     "");
    BOOST_CHECK_EQUAL(tag_vorbis.url(),             "");
}

// Check files fully-tagged (i.e. every field populated) by EasyTag.
BOOST_AUTO_TEST_CASE (easytag_full_metadata)
{
    fs::path flac_path  {file_formats_dir_str + "/chirp.full.easytag.01.flac"};
    fs::path mp4_path   {file_formats_dir_str + "/chirp.full.easytag.02.m4a"};
    fs::path mpeg_path  {file_formats_dir_str + "/chirp.full.easytag.03.mp3"};
    fs::path vorbis_path{file_formats_dir_str + "/chirp.full.easytag.04.ogg"};

    cout << "Testing " << flac_path << endl;
    BOOST_CHECK(fs::exists(flac_path));
    
    mm::metadata tag_flac{flac_path};
    tag_flac.print_properties(cout);
    cout << endl;
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
    tag_mp4.print_properties(cout);
    cout << endl;
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
    
    cout << "Testing " << mpeg_path << endl;
    BOOST_CHECK(fs::exists(mpeg_path));
    
    mm::metadata tag_mpeg{mpeg_path};
    tag_mpeg.print_properties(cout);
    cout << endl;
    BOOST_CHECK_EQUAL(tag_mpeg.album(),           "Album field");
    BOOST_CHECK_EQUAL(tag_mpeg.album_artist(),    "Album artist field");
    BOOST_CHECK_EQUAL(tag_mpeg.artist(),          "Artist field");
    BOOST_CHECK_EQUAL(tag_mpeg.comment(),         "Comment field");
    BOOST_CHECK_EQUAL(tag_mpeg.composer(),        "Composer field");
    BOOST_CHECK_EQUAL(tag_mpeg.copyright(),       "Copyright field");
    BOOST_CHECK_EQUAL(tag_mpeg.encoded_by(),      "Encoded by field");
    BOOST_CHECK_EQUAL(tag_mpeg.date(),            "2017");
    BOOST_CHECK_EQUAL(tag_mpeg.disc_number(),     "1");
    BOOST_CHECK_EQUAL(tag_mpeg.disc_total(),      "2");
    BOOST_CHECK_EQUAL(tag_mpeg.genre(),           "Pop");
    BOOST_CHECK_EQUAL(tag_mpeg.original_artist(), "Original artist field");
    BOOST_CHECK_EQUAL(tag_mpeg.title(),           "Title field");
    BOOST_CHECK_EQUAL(tag_mpeg.track_number(),    "3");
    BOOST_CHECK_EQUAL(tag_mpeg.track_total(),     "4");
    BOOST_CHECK_EQUAL(tag_mpeg.url(),             "URL field");
    
    cout << "Testing " << vorbis_path << endl;
    BOOST_CHECK(fs::exists(vorbis_path));
    
    mm::metadata tag_vorbis{vorbis_path};
    tag_vorbis.print_properties(cout);
    cout << endl;
    BOOST_CHECK_EQUAL(tag_vorbis.album(),           "Album field");
    BOOST_CHECK_EQUAL(tag_vorbis.album_artist(),    "Album artist field");
    BOOST_CHECK_EQUAL(tag_vorbis.artist(),          "Artist field");
    BOOST_CHECK_EQUAL(tag_vorbis.comment(),         "Comment field");
    BOOST_CHECK_EQUAL(tag_vorbis.composer(),        "Composer field");
    BOOST_CHECK_EQUAL(tag_vorbis.copyright(),       "Copyright field");
    BOOST_CHECK_EQUAL(tag_vorbis.encoded_by(),      "Encoded by field");
    BOOST_CHECK_EQUAL(tag_vorbis.date(),            "2017");
    BOOST_CHECK_EQUAL(tag_vorbis.disc_number(),     "1");
    BOOST_CHECK_EQUAL(tag_vorbis.disc_total(),      "2");
    BOOST_CHECK_EQUAL(tag_vorbis.genre(),           "Pop");
    BOOST_CHECK_EQUAL(tag_vorbis.original_artist(), "Original artist field");
    BOOST_CHECK_EQUAL(tag_vorbis.title(),           "Title field");
    BOOST_CHECK_EQUAL(tag_vorbis.track_number(),    "4");
    BOOST_CHECK_EQUAL(tag_vorbis.track_total(),     "4");
    BOOST_CHECK_EQUAL(tag_vorbis.url(),             "URL field");
}

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

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <chaiscript/chaiscript.hpp>

namespace fs = boost::filesystem;
namespace cs = chaiscript;

namespace mm {

std::string get_format_from_script(
        const fs::path &file,
        const metadata &tag,
        const context &ctx)
{
    // Initialise chai
    cs::ChaiScript chai;
    
    // Add bound members from metadata
    chai.add(cs::fun(&metadata::album, &tag), "album");
    chai.add(cs::fun(&metadata::album_artist, &tag), "album_artist");
    chai.add(cs::fun(&metadata::artist, &tag), "artist");
    chai.add(cs::fun(&metadata::comment, &tag), "comment");
    chai.add(cs::fun(&metadata::composer, &tag), "composer");
    chai.add(cs::fun(&metadata::copyright, &tag), "copyright");
    chai.add(cs::fun(&metadata::date, &tag), "date");
    chai.add(cs::fun(&metadata::disc_number, &tag), "disc_number");
    chai.add(cs::fun(&metadata::disc_total, &tag), "disc_total");
    chai.add(cs::fun(&metadata::encoded_by, &tag), "encoded_by");
    chai.add(cs::fun(&metadata::genre, &tag), "genre");
    chai.add(cs::fun(&metadata::original_artist, &tag), "original_artist");
    chai.add(cs::fun(&metadata::title, &tag), "title");
    chai.add(cs::fun(&metadata::track_number, &tag), "track_number");
    chai.add(cs::fun(&metadata::track_total, &tag), "track_total");
    chai.add(cs::fun(&metadata::url, &tag), "url");
    chai.add(cs::const_var(file.string()), "path");
    chai.add(cs::const_var(file.filename().string()), "filename");
    chai.add(cs::const_var(file.parent_path().string()), "parent_dir");

    return chai.eval_file<std::string>(ctx.format_script.string());
}


} // namespace mm

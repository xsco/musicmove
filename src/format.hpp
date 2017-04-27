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
#ifndef MUSICMOVE_FORMAT_HPP
#define MUSICMOVE_FORMAT_HPP

#include <string>
#include <boost/filesystem/path.hpp>
#include "context.hpp"
#include "metadata.hpp"

namespace mm {

std::string convert_for_filesystem(const std::string &str, const context &ctx);

boost::filesystem::path format_path_easytag(
        const boost::filesystem::path &file,
        const std::string &format,
        const metadata &tag,
        const context &ctx);

std::string get_format_from_script(
        const boost::filesystem::path &file,
        const metadata &tag,
        const context &ctx);

} // namespace mm

#endif // MUSICMOVE_FORMAT_HPP

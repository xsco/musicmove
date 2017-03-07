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
#ifndef MUSICMOVE_CONTEXT_HPP
#define MUSICMOVE_CONTEXT_HPP

#include <boost/filesystem/path.hpp>
#include <string>

namespace fs = boost::filesystem;

namespace mm {

enum class path_uniqueness_t { skip, exit };

enum class path_conversion_t { posix, windows_ascii };

struct context
{
    std::string format;
    bool simulate;
    bool verbose;
    bool very_verbose;
    path_uniqueness_t path_uniqueness;
    path_conversion_t path_conversion;
};

} // namespace mm

#endif // MUSICMOVE_CONTEXT_HPP

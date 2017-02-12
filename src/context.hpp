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

struct context
{
    std::string format;
    fs::path base_dir;
    bool simulate;
    bool verbose;
    bool very_verbose;
};

} // namespace mm

#endif // MUSICMOVE_CONTEXT_HPP

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
#ifndef MUSICMOVE_MOVE_HPP
#define MUSICMOVE_MOVE_HPP

#include <boost/filesystem/path.hpp>
#include "context.hpp"

namespace mm {

struct process_results
{
    int files_processed;
    int subdirs_processed;
    bool path_moved;
};

process_results process_path(const boost::filesystem::path &path,
                             const mm::context &ctx);


struct move_results
{
    bool filename_changed;
    bool dir_changed;
};

move_results move_file(const boost::filesystem::path &file,
                       const context &ctx);

} // namespace mm

#endif // MUSICMOVE_MOVE_HPP

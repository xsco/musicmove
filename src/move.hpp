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
#include <string>
#include <stdexcept>
#include "context.hpp"

namespace mm {

struct process_results
{
    process_results() :
        files_processed{0}, dirs_processed{0}, moved_out_of_parent_dir{false}
    {}

    int files_processed;
    int dirs_processed;
    // Was the path moved "out" of its current parent path?
    bool moved_out_of_parent_dir;
};

process_results process_path(const boost::filesystem::path &path,
                             const mm::context &ctx);

struct path_uniqueness_violation : std::runtime_error
{
    explicit path_uniqueness_violation(const std::string &what_arg) :
        std::runtime_error(what_arg)
    {}
};

struct move_results
{
    move_results() :
        filename_changed{false}, dir_changed{false},
        moved_out_of_parent_dir{false}
    {}

    // Was a file renamed within the same dir?
    bool filename_changed;
    // Was a file moved to a different dir?
    bool dir_changed;
    // Was a file moved "out" of its current parent?
    bool moved_out_of_parent_dir;
};

move_results move_file(const boost::filesystem::path &file,
                       const context &ctx);

} // namespace mm

#endif // MUSICMOVE_MOVE_HPP

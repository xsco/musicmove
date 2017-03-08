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
#ifndef MUSICMOVE_METADATA_HPP
#define MUSICMOVE_METADATA_HPP

#include <boost/filesystem.hpp>
#include <string>
#include <memory>

namespace mm {

class metadata
{
public:
    metadata(const boost::filesystem::path &path);
    ~metadata();
    
    bool has_tag() const;
    
    std::string album() const;
    std::string artist() const;
    std::string comment() const;
    std::string date() const;
    std::string genre() const;
    std::string title() const;
    std::string track_number() const;

private:
    class impl;
    class basic_impl;
    class mpeg_impl;
    class flac_impl;
    class ogg_impl;
    class mp4_impl;
    static std::unique_ptr<impl> make_impl(const boost::filesystem::path &path);
    
    std::unique_ptr<impl> impl_;
};

} // namespace mm

#endif // MUSICMOVE_METADATA_HPP

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
#ifndef MUSICMOVE_METADATA_MP4_HPP
#define MUSICMOVE_METADATA_MP4_HPP

#include "metadata.hpp"

#include <mp4file.h>
#include <algorithm>
#include <iostream>

namespace fs = boost::filesystem;

namespace mm {

// MP4 implementation
class metadata::mp4_impl : public metadata::base_impl
{
public:
    mp4_impl(const fs::path &path) :
        base_impl{path}
    {}
    ~mp4_impl() {}
    
    virtual std::string album_artist() const
    {
        // Expect album artist stored as a duplicate artist tag, but for
        // some reason is only accessible directly via the "aART" atom.
        if (!has_tag())
            return "";
        auto *f = dynamic_cast<const TagLib::MP4::File *>(file_ptr());
        auto &item_map = f->tag()->itemListMap();
        if (!item_map.contains("aART"))
            return "";
        auto list = item_map["aART"].toStringList();
        if (list.isEmpty())
            return "";
        return list[0].to8Bit(true);
    }
    virtual std::string disc_number() const
    {
        auto val = base_impl::disc_number();
        // Remove anything after forward slash, if format is "DISC/TOTAL".
        auto pos = val.find_first_of('/');
        if (pos != std::string::npos)
            val.erase(pos);
        return val;
    }
    virtual std::string disc_total() const
    {
        auto val = base_impl::disc_number();
        // Remove up to and including a forward slash if format is DISC/TOTAL.
        auto pos = val.find_first_of('/');
        if (pos != std::string::npos)
            val.erase(0, std::min(pos + 1, val.size()));
        else
            val = "";
        return val;
    }
    virtual std::string track_number() const
    {
        auto val = base_impl::track_number();
        // Remove anything after forward slash, if format is "TRACK/TOTAL".
        auto pos = val.find_first_of('/');
        if (pos != std::string::npos)
            val.erase(pos);
        return val;
    }
    virtual std::string track_total() const
    {
        auto val = base_impl::track_number();
        // Remove up to and including a forward slash if format is TRACK/TOTAL.
        auto pos = val.find_first_of('/');
        if (pos != std::string::npos)
            val.erase(0, std::min(pos + 1, val.size()));
        else
            val = "";
        return val;
    }
};

} // namespace mm

#endif // MUSICMOVE_METADATA_MP4_HPP

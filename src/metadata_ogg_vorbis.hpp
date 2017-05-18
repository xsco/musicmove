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
#ifndef MUSICMOVE_METADATA_OGG_VORBIS_HPP
#define MUSICMOVE_METADATA_OGG_VORBIS_HPP

#include "metadata.hpp"

#include <algorithm>

namespace fs = boost::filesystem;

namespace mm {

class metadata::ogg_vorbis_impl : public metadata::base_impl
{
public:
    ogg_vorbis_impl(const fs::path &path) :
        base_impl{path}
    {}
    ~ogg_vorbis_impl() {}

    virtual std::string comment() const { return get_prop("DESCRIPTION"); }
    virtual std::string encoded_by() const { return get_prop("ENCODED-BY"); }
    virtual std::string track_number() const
    {
        auto val = base_impl::track_number();
        // Remove leading zeroes
        val.erase(0, std::min(val.find_first_not_of('0'), val.size()-1));
        // Remove anything after forward slash, if format is "TRACK/TOTAL".
        auto pos = val.find_first_of('/');
        if (pos != std::string::npos)
            val.erase(pos);
        return val;
    }
    virtual std::string track_total() const
    {
        auto val = base_impl::track_total();
        if (val != "")
        {
            // Remove leading zeroes
            val.erase(0, std::min(val.find_first_not_of('0'), val.size()-1));
            return val;
        }
        
        // Try looking in track number instead
        val = base_impl::track_number();
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

#endif // MUSICMOVE_METADATA_OGG_VORBIS_HPP

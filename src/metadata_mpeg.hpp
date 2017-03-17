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
#ifndef MUSICMOVE_METADATA_MPEG_HPP
#define MUSICMOVE_METADATA_MPEG_HPP

#include "metadata.hpp"

#include <mpegfile.h>
#include <algorithm>

namespace fs = boost::filesystem;

namespace mm {

class metadata::mpeg_impl : public metadata::base_impl
{
public:
    mpeg_impl(const fs::path &path) :
        file_{path.c_str()}
    {}
    ~mpeg_impl() {}

    virtual bool has_tag() const { return file_.hasID3v2Tag() ||
                                          file_.hasID3v1Tag() ||
                                          file_.hasAPETag(); }
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
    virtual std::string original_artist() const { return get_prop("ORIGINALARTIST"); }
    virtual std::string track_number() const
    {
        auto val = base_impl::track_number();
        // Remove anything after forward slash, if format is "TRACK/TOTAL".
        auto pos = val.find_first_of('/');
        if (pos != std::string::npos)
            val.erase(pos);
        // Remove leading zeroes
        val.erase(0, std::min(val.find_first_not_of('0'), val.size()-1));
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
        // Remove leading zeroes
        val.erase(0, std::min(val.find_first_not_of('0'), val.size()-1));
        return val;
    }
    virtual std::string url() const { return get_prop("URL"); }

protected:
    const TagLib::PropertyMap properties() const { return file_.properties(); }
    
private:
    TagLib::MPEG::File file_;
};

} // namespace mm

#endif // MUSICMOVE_METADATA_MPEG_HPP

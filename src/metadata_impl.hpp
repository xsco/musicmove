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
#ifndef MUSICMOVE_METADATA_IMPL_HPP
#define MUSICMOVE_METADATA_IMPL_HPP

#include "metadata.hpp"

#include <tpropertymap.h>

namespace mm {

class metadata::base_impl
{
public:
    base_impl() {}
    virtual ~base_impl() {}
    
    virtual bool has_tag() const = 0;
    virtual std::string album()           const { return get_prop("ALBUM"); }
    virtual std::string album_artist()    const { return get_prop("ALBUMARTIST"); }
    virtual std::string artist()          const { return get_prop("ARTIST"); }
    virtual std::string comment()         const { return get_prop("COMMENT"); }
    virtual std::string composer()        const { return get_prop("COMPOSER"); }
    virtual std::string copyright()       const { return get_prop("COPYRIGHT"); }
    virtual std::string encoded_by()      const { return get_prop("ENCODED-BY"); }
    virtual std::string date()            const { return get_prop("DATE"); }
    virtual std::string disc_number()     const { return get_prop("DISCNUMBER"); }
    virtual std::string disc_total()      const { return get_prop("DISCTOTAL"); }
    virtual std::string genre()           const { return get_prop("GENRE"); }
    virtual std::string original_artist() const { return get_prop("PERFORMER"); }
    virtual std::string title()           const { return get_prop("TITLE"); }
    virtual std::string track_number()    const { return get_prop("TRACKNUMBER"); }
    virtual std::string track_total()     const { return get_prop("TRACKTOTAL"); }
    virtual std::string url()             const { return get_prop("CONTACT"); }

protected:
    virtual const TagLib::PropertyMap properties() const = 0;

    virtual std::string get_prop(const char *name, size_t index = 0) const
    {
        auto vals = properties()[name];
        if (vals.isEmpty())
            return "";
        if (index >= vals.size())
            return "";
        auto val = vals[index];
        // Get UTF-8 encoding of the property value
        return val.to8Bit(true);
    }
};

} // namespace mm

#endif // MUSICMOVE_METADATA_IMPL_HPP

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
#include "metadata.hpp"

#include <fileref.h>
#include <tpropertymap.h>
#include <boost/filesystem.hpp>
#include <memory>
#include <string>
#include <sstream>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace mm {

using std::string;
using std::unique_ptr;
using std::stringstream;
using std::out_of_range;

class metadata::impl
{
public:
    virtual ~impl() = default;
    virtual bool has_tag() const = 0;
    virtual string album() const = 0;
    virtual string artist() const = 0;
    virtual string comment() const = 0;
    virtual string date() const = 0;
    virtual string genre() const = 0;
    virtual string title() const = 0;
    virtual string track_number() const = 0;
    // TODO add additional members, and maybe make basic_impl abstract base class
};

class metadata::basic_impl : public metadata::impl
{
public:
    basic_impl(const fs::path &path) :
        fileref_{path.c_str()},
        tag_{fileref_.tag()}
    {}
    ~basic_impl() {}
    
    virtual bool has_tag() const { return tag_ != nullptr; }
    virtual string album() const { return get_prop("ALBUM"); }
    virtual string artist() const { return get_prop("ARTIST"); }
    virtual string comment() const { return get_prop("COMMENT"); }
    virtual string date() const { return get_prop("DATE"); }
    virtual string genre() const { return get_prop("GENRE"); }
    virtual string title() const { return get_prop("TITLE"); }
    virtual string track_number() const { return get_prop("TRACKNUMBER"); }

private:

    string get_prop(const char *name) const
    {
        if (tag_ == nullptr)
            return "";
        auto vals = tag_->properties()[name];
        if (vals.isEmpty())
            return "";
        auto val = vals[0];
        // Get UTF-8 encoding of the property value
        return val.to8Bit(true);
    }

    const TagLib::FileRef fileref_;
    const TagLib::Tag *tag_;
};


unique_ptr<metadata::impl> metadata::make_impl(const fs::path &path)
{
    // TODO - select metadata impl based on file extension (case sensitivity?)
    /*
    auto ext = path.extension();
    if (ext == ".flac")
        return unique_ptr<metadata::impl>{new flac_impl};
    */
    
    return unique_ptr<metadata::impl>{new basic_impl{path}};
    /*
    stringstream err_msg;
    err_msg << "Unrecognised file extension "
            << ext
            << " in path "
            << path;
    throw out_of_range{err_msg.str().c_str()};
    */
}

metadata::metadata(const fs::path &path) :
    impl_{make_impl(path)}
{}

metadata::~metadata()
{}

bool   metadata::has_tag()      const { return impl_->has_tag(); }
string metadata::album()        const { return impl_->album(); }
string metadata::artist()       const { return impl_->artist(); }
string metadata::comment()      const { return impl_->comment(); }
string metadata::date()         const { return impl_->date(); }
string metadata::genre()        const { return impl_->genre(); }
string metadata::title()        const { return impl_->title(); }
string metadata::track_number() const { return impl_->track_number(); }

} // namespace mm

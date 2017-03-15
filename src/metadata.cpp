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
#include "metadata_impl.hpp"

#include <fileref.h>
#include <flacfile.h>
#include <mp4file.h>
#include <tpropertymap.h>
#include <boost/filesystem.hpp>
#include <memory>
#include <string>
#include <sstream>
#include <algorithm>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace mm {

using std::string;
using std::unique_ptr;
using std::stringstream;
using std::out_of_range;
using std::cout;
using std::endl;

// FLAC implementation
// TODO - create header (all inline?) and move flac_impl to separate file
class metadata::flac_impl : public metadata::base_impl
{
public:
    flac_impl(const fs::path &path) :
        file_{path.c_str()}
    {
        /*
        for (auto &kvp : file_.properties())
        {
            for (auto &val : kvp.second)
            {
                cout << kvp.first << " -> " << val << endl;
            }
        }
        */
    }
    ~flac_impl() {}
    
    virtual bool has_tag() const { return file_.hasXiphComment() ||
                                          file_.hasID3v2Tag() ||
                                          file_.hasID3v1Tag(); }
    virtual string comment() const { return get_prop("DESCRIPTION"); }
    virtual string track_number() const
    {
        auto val = base_impl::track_number();
        // Remove leading zeroes
        val.erase(0, std::min(val.find_first_not_of('0'), val.size()-1));
        return val;
    }
    virtual string track_total() const
    {
        auto val = base_impl::track_total();
        // Remove leading zeroes
        val.erase(0, std::min(val.find_first_not_of('0'), val.size()-1));
        return val;
    }

protected:
    const TagLib::PropertyMap properties() const { return file_.properties(); }
    
private:
    TagLib::FLAC::File file_;
};

// MP4 implementation
// TODO - create header (all inline?) and move flac_impl to separate file
class metadata::mp4_impl : public metadata::base_impl
{
public:
    mp4_impl(const fs::path &path) :
        file_{path.c_str()}
    {
        for (auto &kvp : file_.properties())
        {
            for (auto &val : kvp.second)
            {
                cout << kvp.first << " -> " << val << endl;
            }
        }
        cout << "--" << endl;
        for (auto &kvp : file_.tag()->itemListMap())
        {
            for (auto entry : kvp.second.toStringList())
            {
                cout << kvp.first << " -> " << entry << endl;
            }
        }
    }
    ~mp4_impl() {}
    
    virtual bool has_tag() const { return file_.tag() != nullptr; }
    virtual string album_artist() const
    {
        // Expect album artist stored as a duplicate artist tag, but for
        // some reason is only accessible directly via the "aART" atom.
        if (!has_tag())
            return "";
        auto &item_map = file_.tag()->itemListMap();
        if (!item_map.contains("aART"))
            return "";
        auto list = item_map["aART"].toStringList();
        if (list.isEmpty())
            return "";
        return list[0].to8Bit(true);
    }
    virtual string disc_number() const
    {
        auto val = base_impl::disc_number();
        // Remove anything after forward slash, if format is "DISC/TOTAL".
        val.erase(val.find_first_of('/'));
        return val;
    }
    virtual string disc_total() const
    {
        auto val = base_impl::disc_number();
        // Remove up to and including a forward slash if format is DISC/TOTAL.
        val.erase(0, std::min(val.find_first_of('/') + 1, val.size()));
        return val;
    }
    virtual string encoded_by() const { return get_prop("ENCODEDBY"); }
    virtual string track_number() const
    {
        auto val = base_impl::track_number();
        // Remove anything after forward slash, if format is "TRACK/TOTAL".
        val.erase(val.find_first_of('/'));
        return val;
    }
    virtual string track_total() const
    {
        auto val = base_impl::track_number();
        // Remove up to and including a forward slash if format is TRACK/TOTAL.
        val.erase(0, std::min(val.find_first_of('/') + 1, val.size()));
        return val;
    }

protected:
    const TagLib::PropertyMap properties() const
    {
        //return file_.properties();
        return file_.tag()->properties();
    }
    
private:
    TagLib::MP4::File file_;
};


unique_ptr<metadata::base_impl> metadata::make_impl(const fs::path &path)
{
    // Select metadata impl based on file extension
    auto ext = path.extension();
    if (ext == ".flac")
        return unique_ptr<metadata::base_impl>{new flac_impl{path}};
    if (ext == ".m4a")
        return unique_ptr<metadata::base_impl>{new mp4_impl{path}};

    // TODO - other filetype metadata implementations
    
    stringstream err_msg;
    err_msg << "Unrecognised file extension "
            << ext
            << " in path "
            << path;
    throw out_of_range{err_msg.str().c_str()};
}

metadata::metadata(const fs::path &path) :
    impl_{make_impl(path)}
{}

metadata::~metadata()
{}

bool   metadata::has_tag()         const { return impl_->has_tag(); }
string metadata::album()           const { return impl_->album(); }
string metadata::album_artist()    const { return impl_->album_artist(); }
string metadata::artist()          const { return impl_->artist(); }
string metadata::comment()         const { return impl_->comment(); }
string metadata::composer()        const { return impl_->composer(); }
string metadata::copyright()       const { return impl_->copyright(); }
string metadata::encoded_by()      const { return impl_->encoded_by(); }
string metadata::date()            const { return impl_->date(); }
string metadata::disc_number()     const { return impl_->disc_number(); }
string metadata::disc_total()      const { return impl_->disc_total(); }
string metadata::genre()           const { return impl_->genre(); }
string metadata::original_artist() const { return impl_->original_artist(); }
string metadata::title()           const { return impl_->title(); }
string metadata::track_number()    const { return impl_->track_number(); }
string metadata::track_total()     const { return impl_->track_total(); }
string metadata::url()             const { return impl_->url(); }

} // namespace mm

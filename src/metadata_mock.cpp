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

#include <boost/filesystem.hpp>
#include <algorithm>
#include <string>
#include <stdexcept>

// This is a MOCK implementation of the classes and functions in metadata.hpp

namespace fs = boost::filesystem;

namespace mm {

using std::string;

// MOCK - empty base_impl class
class metadata::base_impl
{
public:
    base_impl(const fs::path &path) :
        has_tag_{path.extension() == ".inc"}
    {}

    bool has_tag() const { return has_tag_; }

private:
    bool has_tag_;
};

std::unique_ptr<metadata::base_impl> metadata::make_impl(const fs::path &path)
{
    return std::unique_ptr<metadata::base_impl>{new metadata::base_impl{path}};
}

metadata::metadata(const fs::path &path) :
    impl_{make_impl(path)}
{}

metadata::~metadata()
{}

void metadata::print_properties(std::ostream &os)
{
    // MOCK - do nothing
}

// MOCK - return a fixed string for all tags
bool   metadata::has_tag()         const { return impl_->has_tag(); }
string metadata::album()           const { return "album"; }
string metadata::album_artist()    const { return "albumartist"; }
string metadata::artist()          const { return "artist"; }
string metadata::comment()         const { return "comment"; }
string metadata::composer()        const { return "composer"; }
string metadata::copyright()       const { return "copyright"; }
string metadata::encoded_by()      const { return "encodedby"; }
string metadata::date()            const { return "date"; }
string metadata::disc_number()     const { return "discnumber"; }
string metadata::disc_total()      const { return "disctotal"; }
string metadata::genre()           const { return "genre"; }
string metadata::original_artist() const { return "originalartist"; }
string metadata::title()           const { return "title"; }
string metadata::track_number()    const { return "tracknumber"; }
string metadata::track_total()     const { return "tracktotal"; }
string metadata::url()             const { return "url"; }

} // namespace mm

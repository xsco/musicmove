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
{};

std::unique_ptr<metadata::base_impl> metadata::make_impl(const fs::path &path)
{
    throw std::runtime_error{"Code should never be called"};
}

metadata::metadata(const fs::path &path) :
    impl_{}
{}

metadata::~metadata()
{}

void metadata::print_properties(std::ostream &os)
{
    // MOCK - do nothing
}

// MOCK - return empty string for all tags
bool   metadata::has_tag()         const { return true; }
string metadata::album()           const { return ""; }
string metadata::album_artist()    const { return ""; }
string metadata::artist()          const { return ""; }
string metadata::comment()         const { return ""; }
string metadata::composer()        const { return ""; }
string metadata::copyright()       const { return ""; }
string metadata::encoded_by()      const { return ""; }
string metadata::date()            const { return ""; }
string metadata::disc_number()     const { return ""; }
string metadata::disc_total()      const { return ""; }
string metadata::genre()           const { return ""; }
string metadata::original_artist() const { return ""; }
string metadata::title()           const { return ""; }
string metadata::track_number()    const { return ""; }
string metadata::track_total()     const { return ""; }
string metadata::url()             const { return ""; }

} // namespace mm

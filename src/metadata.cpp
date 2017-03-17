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
#include "metadata_base.hpp"
#include "metadata_flac.hpp"
#include "metadata_mp4.hpp"
#include "metadata_mpeg.hpp"
#include "metadata_ogg_vorbis.hpp"

#include <boost/filesystem.hpp>
#include <memory>
#include <algorithm>
#include <string>
#include <sstream>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace mm {

using std::string;

std::unique_ptr<metadata::base_impl> metadata::make_impl(const fs::path &path)
{
    // Select metadata impl based on file extension (assume lowercase ASCII)
    string ext{path.extension().c_str()};
    std::transform(std::begin(ext), std::end(ext), std::begin(ext), ::tolower);
    if (ext == ".flac")
        return std::unique_ptr<metadata::base_impl>{new flac_impl{path}};
    if (ext == ".m4a")
        return std::unique_ptr<metadata::base_impl>{new mp4_impl{path}};
    if (ext == ".mp3")
        return std::unique_ptr<metadata::base_impl>{new mpeg_impl{path}};
    if (ext == ".ogg")
        return std::unique_ptr<metadata::base_impl>{new ogg_vorbis_impl{path}};
    
    std::stringstream err_msg;
    err_msg << "Unrecognised file extension "
            << ext
            << " in path "
            << path;
    throw std::out_of_range{err_msg.str().c_str()};
}

metadata::metadata(const fs::path &path) :
    impl_{make_impl(path)}
{}

metadata::~metadata()
{}

void metadata::print_properties(std::ostream &os)
{
    impl_->print_properties(os);
}

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

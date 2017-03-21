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
#include "format.hpp"

#include <boost/locale.hpp>
#include <algorithm>
#include <regex>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace mm {

using std::string;

string convert_for_filesystem(const string &str, const context &ctx)
{
    // Make the string suitable for writing as a path to the filesystem
    // Assume it is in UTF-8.
    string safe;
    
    if (ctx.path_conversion == path_conversion_t::utf8)
    {
        // We want to keep the path element as UTF-8.  We will only remove
        // typical directory separators, therefore.
        safe = std::regex_replace(str, std::regex{"[/\\\\]"}, "");
    }
    else
    {
        // First, convert to 8-bit Latin1
        safe = boost::locale::conv::from_utf(str, "Latin1");
    
        // Remove marked characters using a small lookup table
        // This is very crude, and not linguistically accurate, but can be
        // argued to suffice for conversion to a 'safe' filesystem path
        const char *tr =
            "AAAAAAECEEEEIIIIDNOOOOO*OUUUUYPsaaaaaaeceeeeiiiionooooo/ouuuuypy";
        //  "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ"
        std::transform(safe.begin(), safe.end(), safe.begin(),
            [&tr](char &sc) {
                unsigned char &c = reinterpret_cast<unsigned char &>(sc);
                return c >= 192 ? tr[c - 192] : sc;
            });
    }
    
    // Remove any non-portable characters
    if (ctx.path_conversion == path_conversion_t::posix)
    {
        std::regex posix_exp{"[^A-Za-z0-9\\._-]"};
        safe = std::regex_replace(safe, posix_exp, "_");
    }
    else if (ctx.path_conversion == path_conversion_t::windows_ascii)
    {
        std::regex windows_exp{"[<>:\"/\\|]"};
        safe = std::regex_replace(safe, windows_exp, "_");
    }
    else
        throw std::out_of_range("ASSERT: unknown value of path_conversion_t"
            " not handled!");

    // Trim trailing underscores
    safe.erase(
        std::find_if(safe.rbegin(), safe.rend(),
                     [](auto c) { return c != '_'; }).base(),
        safe.end());
    return safe;
}

} // namespace mm

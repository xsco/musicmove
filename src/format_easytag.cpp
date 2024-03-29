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

#include <iostream>
#include <sstream>
#include <regex>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace mm {

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;
using std::for_each;

static string get_token_easytag(const metadata &tag, const char c)
{
    // %a - Track artist
    // %b - Album
    // %c - Comment
    // %d - Disc number
    // %e - Encoded by
    // %g - Genre
    // %l - Number of tracks
    // %n - Track number
    // %o - Original artist
    // %p - Composer
    // %r - Copyright
    // %t - Track title
    // %u - URL
    // %x - Number of discs
    // %y - Year
    // %z - Album artist
    string val;
    stringstream err_msg;
    switch (c)
    {
        case 'a':
            return tag.artist();
        case 'b':
            return tag.album();
        case 'c':
            return tag.comment();
        case 'd':
            return tag.disc_number();
        case 'e':
            return tag.encoded_by();
        case 'g':
            return tag.genre();
        case 'l':
            // Pad with zero if the number is only one character long
            val = tag.track_total();
            if (val.length() == 1)
                val.insert(0, 1, '0');
            return val;
        case 'n':
            // Pad with zero if the number is only one character long
            val = tag.track_number();
            if (val.length() == 1)
                val.insert(0, 1, '0');
            return val;
        case 'o':
            return tag.original_artist();
        case 'p':
            return tag.composer();
        case 'r':
            return tag.copyright();
        case 't':
            return tag.title();
        case 'u':
            return tag.url();
        case 'x':
            return tag.disc_total();
        case 'y':
            return tag.date();
        case 'z':
            // Fall back to artist if album artist is not available
            val = tag.album_artist();
            if (val == "")
                return tag.artist();
            return val;
        case '%':
            // It's an actual percentage sign in the filename!
            return "%";
        default:
            err_msg << "Unknown format specifier `%" << c << "'";
            throw std::out_of_range(err_msg.str().c_str());
    }
}

fs::path format_path_easytag(const fs::path &file, const string &format,
                             const metadata &tag, const context &ctx)
{
    // Replace tokens in a format string.  Expect EasyTag-style expressions
    // where each token is a '%' symbol followed by a single letter

    string new_path_str;
    string::size_type len = format.length();
    string::size_type last_start = 0;
    new_path_str.reserve(format.length());
    for (auto found = format.find_first_of('%');
         found != string::npos;
         last_start = found + 1, found = format.find_first_of('%', found + 1))
    {
        // Copy anything found so far to the new path
        if (found > 0 && found > last_start)
            new_path_str.append(format, last_start, found - last_start);
        
        // Ensure no unfinished tokens at end of string
        ++found;
        if (found == len)
        {
            throw std::invalid_argument(
                "Unmatched `%' sign at end of format string");
        }
    
        auto c = format[found];
        // Decode the token, make sure it doesn't contain any path separator
        // characters, and append to the path
        new_path_str.append(std::regex_replace(
            get_token_easytag(tag, c), std::regex{"[/\\\\]"}, "-"));
    }
    // If the string was empty, it means we didn't find any % tokens
    // In such a case, just use the format as a hard-coded path
    if (new_path_str == "")
        new_path_str = format;
    
    // Construct the path, and make sure each element is suitable for writing
    // to the filesystem
    fs::path unconverted_path{new_path_str};
    fs::path new_path;
    for (auto &path_elem : unconverted_path)
    {
        if (path_elem.is_absolute())
        {
            // Ensure the root path is at the beginning of the path
            new_path = path_elem.root_path();
            new_path /= path_elem.relative_path();
        }
        else
        {
            // Append
            auto part = convert_for_filesystem(path_elem.string(), ctx);
            new_path /= part;
        }
    }
    
    // Work out the full path for the renamed file.
    // If the format does not specify a directory, it remains in the same
    // dir; if the format specifies a relative path, it is relative to the
    // file's current directory (apart from dot and dot-dot); if the format
    // specifies an absolute path, then it is used as-is.
    if (new_path.is_relative())
    {
        // Do we have a relative dir specifier like dot or dot-dot?
        // If so, convert to absolute based on the current working directory.
        if (new_path.has_parent_path() &&
            !new_path_str.empty() && new_path_str[0] == '.')
            // Path actually starts from CWD like an absolute path
            new_path = fs::current_path() / new_path;
        else
            // Path is relative.  This means relative to its current directory
            new_path = file.parent_path() / new_path;
    }
    
    // Add file extension
    // TODO check if the new_path already has the file extension on
    new_path += file.extension();
    return new_path;
}

} // namespace mm

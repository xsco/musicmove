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
#include "move.hpp"

#include <boost/filesystem.hpp>
#include <boost/locale.hpp>
#include <fileref.h>
#include <tpropertymap.h>

#include <algorithm>
#include <regex>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <config.h>

namespace fs = boost::filesystem;

namespace mm {

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;
using std::for_each;

process_results process_path(const fs::path &p, const mm::context &ctx)
{
    process_results results;
    results.files_processed = 0;
    results.subdirs_processed = 0;
    results.path_moved = false;
    
    if (!fs::exists(p))
    {
        cerr << "Warning: path does not exist: " << p << endl;
        return results;
    }

    // Regular file or dir?
    if (fs::is_directory(p))
    {
        // It's a directory.  Recurse over everything within.
        ++results.subdirs_processed;
        int file_count = 0;
        for_each(fs::directory_iterator{p},
                 fs::directory_iterator{},
                 [&ctx, &results, &file_count](auto &de)
                 {
                    ++file_count;
                    auto sub_results = process_path(de.path(), ctx);
                    results.files_processed += sub_results.files_processed;
                    results.subdirs_processed += sub_results.subdirs_processed;
                    if (sub_results.path_moved)
                        --file_count;
                 });
        
        // Is the directory now empty?
        if (file_count == 0)
        {
            if (ctx.simulate || ctx.verbose)
                cout << "Remove empty directory " << p << endl;
            
            if (!ctx.simulate)
            {
                // Safety check to make sure the dir is definitely empty
                if (fs::directory_iterator{p} != fs::directory_iterator{})
                {
                    cerr << "Expected " << p << "to be an empty directory "
                            "after music out of it, but it is not empty.  "
                            "This is a bug and should be reported to "
                            PACKAGE_BUGREPORT << endl;
                }
                else
                {
                    fs::remove_all(p);
                }
            }
            results.path_moved = true;
        }
    }
    else
    {
        // Read as a file
        auto sub_results = move_file(p, ctx);
        // Update results for the path
        ++results.files_processed;
        if (sub_results.dir_changed)
            results.path_moved = true;
    }
    
    return results;
};

static void print_file_and_tags(std::ostream &os, const fs::path &file,
                                const TagLib::Tag &tag, const context &ctx)
{
    os << "Considering file " << file << endl;
    
    auto props = tag.properties();
    if (props.isEmpty())
    {
        os << "  (No tags)" << endl;
    }
    else
    {
        for (auto i = props.begin(); i != props.end(); ++i)
        {
            auto field_name = i->first;
            auto values = i->second;
            stringstream ss;
            if (values.isEmpty())
            {
                ss << "(empty)";
            }
            else
            {
                bool first = true;
                for (auto value : values)
                {
                    if (first)
                    {
                        first = false;
                    }
                    else
                    {
                        ss << ", ";
                    }
                    ss << "\"" << value << "\"";
                }
            }
            os << " " << field_name << ": " << ss.str() << endl;
        }
    }
}

static string convert_for_filesystem(const string &str, const context &ctx)
{
    // Make the string suitable for writing as a path to the filesystem
    // Assume it is in UTF-8.
    
    // First, convert to 8-bit Latin1
    string safe = boost::locale::conv::from_utf(str, "Latin1");
    
    // Remove marked characters using a small lookup table
    const char *
        //   "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ"
        tr = "AAAAAAECEEEEIIIIDNOOOOOx0UUUUYPsaaaaaaeceeeeiiiiOnooooo/0uuuuypy";
    std::transform(safe.begin(), safe.end(), safe.begin(),
        [&tr](char &sc) {
            unsigned char &c = reinterpret_cast<unsigned char &>(sc);
            return c >= 192 ? tr[c - 192] : sc;
        });
    
    // Remove any non-portable characters
    if (ctx.path_conversion == path_conversion_t::posix)
    {
        std::regex posix_exp{"[^A-Za-z0-9\\.-_]"};
        return std::regex_replace(safe, posix_exp, "_");
    }
    else if (ctx.path_conversion == path_conversion_t::windows_ascii)
    {
        std::regex windows_exp{"[<>:\"/\\|]"};
        return std::regex_replace(safe, windows_exp, "_");
    }
    else
        throw std::out_of_range("ASSERT: unknown value of path_conversion_t"
            " not handled!");
}

static string get_property_safe(const TagLib::Tag &tag, const char *name,
                                unsigned int prop_offset = 0)
{
    auto vals = tag.properties()[name];
    if (vals.isEmpty())
        return "";
    // For multiple tag values, select the desired offset
    if (prop_offset >= vals.size())
        // Offset not available.  Get the first instead
        prop_offset = 0;
    auto val = vals[prop_offset];
    // Get UTF-8 encoding of the property value
    return val.to8Bit(true);
}

static string get_token_easytag(const TagLib::Tag &tag, const char c)
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
    string::size_type i;
    stringstream err_msg;
    switch (c)
    {
        case 'a':
            return get_property_safe(tag, "ARTIST");
        case 'b':
            return get_property_safe(tag, "ALBUM");
        case 'c':
            return get_property_safe(tag, "COMMENT");
        case 'd':
            return get_property_safe(tag, "DISCNUMBER");
        case 'e':
            return get_property_safe(tag, "ENCODEDBY");
        case 'g':
            return get_property_safe(tag, "GENRE");
        case 'l':
            // Get total tracks from either TRACKTOTAL tag or TRACKNUMBER
            val = get_property_safe(tag, "TRACKTOTAL");
            if (val != "")
                return val;
            val = get_property_safe(tag, "TRACKNUMBER");
            // Look for / and get bit afterwards
            i = val.find('/');
            return i == string::npos ? "" : val.substr(i + 1);
        case 'n':
            // Pad with zero if the number is only one character long
            val = get_property_safe(tag, "TRACKNUMBER");
            if (val.length() == 1)
                val.insert(0, 1, '0');
            return val;
        case 'o':
            // Original artist tag
            return get_property_safe(tag, "PERFORMER");
            //throw std::out_of_range("The %o token for Original Artist is not currently supported");
        case 'p':
            return get_property_safe(tag, "COMPOSER");
        case 'r':
            return get_property_safe(tag, "COPYRIGHT");
        case 't':
            return get_property_safe(tag, "TITLE");
        case 'u':
            // URL tag
            return get_property_safe(tag, "CONTACT");
            //throw std::out_of_range("The %u token for URL is not currently supported");
        case 'x':
            // Get number of discs tag from DISCTOTAL or DISCNUMBER
            val = get_property_safe(tag, "DISCTOTAL");
            if (val != "")
                return val;
            val = get_property_safe(tag, "DISCNUMBER");
            // Look for / and get bit afterwards
            i = val.find('/');
            return i == string::npos ? "" : val.substr(i + 1);
        case 'y':
            return get_property_safe(tag, "DATE");
        case 'z':
            // Fall back to artist if 'albumartist' is not present
            val = get_property_safe(tag, "ALBUMARTIST");
            return val.empty()
                ? get_property_safe(tag, "ARTIST")
                : val;
        case '%':
            // It's an actual percentage sign in the filename!
            return "%";
        default:
            err_msg << "Unknown format specifier `%" << c << "'";
            throw std::out_of_range(err_msg.str().c_str());
    }
}

static fs::path format_path_easytag(const fs::path &file, const string &format,
                            const TagLib::Tag &tag, const context &ctx)
{
    // Replace tokens in a format string.  Expect EasyTag-style expressions
    // where each token is a '%' symbol followed by a single letter

    auto props = tag.properties();
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
        // Decode the token and append to the path
        new_path_str.append(
            convert_for_filesystem(get_token_easytag(tag, c), ctx));
    }
    
    // Work out the full path for the renamed file.
    // If the format does not specify a directory, it remains in the same
    // dir; if the format specifies a relative path, it is relative to the
    // file's current directory (apart from dot and dot-dot); if the format
    // specifies an absolute path, then it is used as-is.
    fs::path new_path{new_path_str};
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
    new_path += file.extension();
    return new_path;
}

move_results move_file(const fs::path &file, const context &ctx)
{
    TagLib::FileRef f{file.c_str()};
    move_results results;
    results.filename_changed = false;
    results.dir_changed = false;
    
    // Does this file have any audio properties?
    if (f.audioProperties() == nullptr)
    {
        if (ctx.very_verbose)
        {
            cerr << "No audio properties found for file "
                 << file << ".. skipping" << endl;
        }
        return results;
    }
    
    // Does this file have a tag?
    auto *tag_ptr = f.tag();
    if (tag_ptr == nullptr)
    {
        if (ctx.verbose)
            cerr << "No tag found for file " << file << ".. skipping" << endl;
        return results;
    }
    auto &tag = *tag_ptr;
    
    if (ctx.very_verbose)
        print_file_and_tags(cout, file, tag, ctx);
    
    auto new_file = format_path_easytag(file, ctx.format, tag, ctx);
    
    // See if the new path is actually any different
    if (new_file == file)
    {
        // No change in the file's path.
        if (ctx.verbose)
            cout << "No change in path for " << file << endl;
        return results;
    }

    // Check to see if new path already exists
    if (fs::exists(new_file))
    {
        // Clash with destination path.
        if (ctx.path_uniqueness == path_uniqueness_t::skip)
        {
            cout << "Warning: want to move " << file << " to " << new_file
                 << ", but that path already exists.  Skipping for now.."
                 << endl;
            return results;
        }
        else if (ctx.path_uniqueness == path_uniqueness_t::exit)
        {
            stringstream msg;
            msg << "Tried to move " << file << " to " << new_file
                 << ", but that path already exists";
            throw std::runtime_error(msg.str().c_str());
        }
        else
            throw std::out_of_range("ASSERT: Unknown value of "
                "path_uniqueness_t not handled!");
    }
    
    if (file.parent_path() != new_file.parent_path())
        results.dir_changed = true;
    if (file.filename() != new_file.filename())
        results.filename_changed = true;
    
    if (ctx.simulate || ctx.verbose)
    {
        if (results.dir_changed && results.filename_changed)
            cout << "Move/rename " << file << " to " << new_file << endl;
        else if (results.dir_changed)
            cout << "Move " << file << " to " << new_file.parent_path() << endl;
        else
            cout << "Rename " << file << " to " << new_file.filename() << endl;
    }
    
    if (!ctx.simulate)
    {
        // TODO ensure parent directory path exists before renaming
        fs::rename(file, new_file);
    }

    return results;
}

} // namespace mm

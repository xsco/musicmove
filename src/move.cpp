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
#include <fileref.h>
#include <tpropertymap.h>

#include <algorithm>
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
            if (ctx.simulate)
                cout << "Would remove empty directory " << p << endl;
            else
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

static fs::path format_path_easytag(const fs::path &file, const string &format,
                            const TagLib::Tag &tag, const context &ctx)
{
    // Replace tokens in a format string.  Expect EasyTag-style expressions
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

    auto props = tag.properties();
    string new_path;
    string::size_type len = format.length();
    string::size_type last_start = 0;
    stringstream err_msg;
    new_path.reserve(format.length());
    for (auto found = format.find_first_of('%');
         found != string::npos;
         last_start = found + 1, found = format.find_first_of('%', found + 1))
    {
        // Copy anything found so far to the new path
        if (found > 0 && found > last_start)
            new_path.append(format, last_start, found - last_start);
        
        // Ensure no unfinished tokens at end of string
        ++found;
        if (found == len)
        {
            err_msg << "Unmatched `%' sign at end of format string";
            throw std::invalid_argument(err_msg.str().c_str());
        }
    
        auto c = format[found];
        string val;
        switch (c)
        {
            case 'a':
                new_path.append(get_property_safe(tag, "ARTIST"));
                break;
            case 'b':
                new_path.append(get_property_safe(tag, "ALBUM"));
                break;
            case 'c':
                new_path.append(get_property_safe(tag, "COMMENT"));
                break;
            case 'd':
                new_path.append(get_property_safe(tag, "DISCNUMBER"));
                break;
            case 'e':
                new_path.append(get_property_safe(tag, "ENCODEDBY"));
                break;
            case 'g':
                new_path.append(get_property_safe(tag, "GENRE"));
                break;
            case 'l':
                // TODO get number of tracks from either dedicated tag or TRACKNUMBER
                //new_path.append(get_property_safe(tag, "")); // Number of tracks?
                break;
            case 'n':
                // Pad with zero if the number is only one character long
                val = get_property_safe(tag, "TRACKNUMBER");
                if (val.length() == 1)
                    val.insert(0, 1, '0');
                new_path.append(val);
                break;
            case 'o':
                // TODO get original artist tag
                //new_path.append(get_property_safe(tag, "")); // Original artist?
                break;
            case 'p':
                new_path.append(get_property_safe(tag, "COMPOSER"));
                break;
            case 'r':
                new_path.append(get_property_safe(tag, "COPYRIGHT"));
                break;
            case 't':
                new_path.append(get_property_safe(tag, "TITLE"));
                break;
            case 'u':
                // TODO get URL tag
                //new_path.append(get_property_safe(tag, "")); // URL?
                break;
            case 'x':
                // TODO get number of discs tag from somewhere
                //new_path.append(get_property_safe(tag, "")); // Number of discs?
                break;
            case 'y':
                new_path.append(get_property_safe(tag, "DATE"));
                break;
            case 'z':
                // Fall back to artist if 'albumartist' is not present
                val = get_property_safe(tag, "ALBUMARTIST");
                new_path.append(val.empty()
                    ? get_property_safe(tag, "ARTIST")
                    : val);
                break;
            case '%':
                // It's an actual percentage sign in the filename!
                new_path.append("%");
                break;
            default:
                err_msg << "Unknown format specifier `%" << c << "'";
                throw std::out_of_range(err_msg.str().c_str());
                break;
        }
    }

    auto final_path = ctx.base_dir;
    final_path /= new_path;
    final_path += file.extension();
    return final_path;
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
    
    // TODO - check filename and directory name validity, according to Boost:
    // www.boost.org/doc/libs/1_58_0/libs/filesystem/doc/portability_guide.htm
    
    // TODO - convert weird characters in filenames

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
        // TODO - Clash with destination path.  Try to make it unique?
        cout << "Warning: want to move " << file << " to " << new_file
             << ", but that path already exists!  Skipping for now.." << endl;
        return results;
    }
    
    if (ctx.simulate)
    {
        cout << "Would rename.." << endl;
        cout << "- from :" << file << endl;
        cout << "- to   :" << new_file << endl;
    }
    else
    {
        if (ctx.verbose)
            cout << "Renaming from " << file << " to " << new_file << endl;
        // TODO - actually rename the file
    }

    if (file.parent_path() != new_file.parent_path())
        results.dir_changed = true;
    if (file.filename() != new_file.filename())
        results.filename_changed = true;
    
    return results;
}

} // namespace mm

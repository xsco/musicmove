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
#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cerrno>

#include <config.h>

#include "metadata.hpp"
#include "format.hpp"


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
    
    if (!fs::exists(p))
    {
        cerr << "Warning: path does not exist: " << p.string() << endl;
        return results;
    }

    // Regular file or dir?
    if (fs::is_directory(p))
    {
        // It's a directory.  Recurse over everything within.
        int dir_entry_count = 0;
        ++results.dirs_processed;
        
        // It is unspecified behaviour what happens to directory_iterators if
        // new subdirs are added to this path by the recursive calls.  We do not
        // need to process any such newly-added subdirs (they'll already be
        // perfectly named, by definition), so we will copy the list of entries
        // in advance and then iterate over that.
        std::vector<fs::path> sub_paths;
        std::for_each(
            fs::directory_iterator{p},
            fs::directory_iterator{},
            [&sub_paths](auto &de) { sub_paths.push_back(de.path()); });
        
        std::for_each(
            std::begin(sub_paths),
            std::end(sub_paths),
            [&ctx, &results, &dir_entry_count](auto &sub_path)
            {
                ++dir_entry_count;
                // Process the sub-directory
                auto sub_results = process_path(sub_path, ctx);
                results.files_processed += sub_results.files_processed;
                results.dirs_processed += sub_results.dirs_processed;
                if (sub_results.moved_out_of_parent_dir)
                    --dir_entry_count;
            });
        
        // Is the directory now potentially empty?
        if (dir_entry_count == 0)
        {
            if (ctx.simulate || ctx.verbose)
                cout << "Considering removal of potentially-empty directory "
                     << p.string() << ".. ";
            
            if (!ctx.simulate)
            {
                // Potentially, the recursive calls to process_path could have
                // created brand new subdirs in this directory, in which case
                // we should not attempt removal.  Only attempt removal if the
                // directory is genuinely empty.
                // Because we rely on the filesystem to help us with this step,
                // this is currently a shortcoming of the `simulate' option.
                if (fs::directory_iterator{p} == fs::directory_iterator{})
                {
                    // Remove this dir and all its contents
                    if (ctx.verbose)
                        cout << "empty" << endl;
                    fs::remove_all(p);
                    results.moved_out_of_parent_dir = true;
                }
                else
                {
                    if (ctx.verbose)
                        cout << "not empty" << endl;
                }
            }
            else
            {
                cout << endl;
            }
        }
    }
    else
    {
        // Read as a file
        auto file_results = move_file(p, ctx);
        // Update results for the path
        ++results.files_processed;
        results.moved_out_of_parent_dir = file_results.moved_out_of_parent_dir;
    }
    
    return results;
};

static bool path_contains(const fs::path &parent, const fs::path &child)
{
    // Work out if a child path is a descendant of a parent path
    auto ip = parent.begin(), ic = child.begin();
    while (ip != parent.end())
    {
        // If we reach the end of the child path, then it can't be contained
        if (ic == child.end())
            return false;
        
        // Is there a mismatch?
        if (*ip != *ic)
            return false;
        
        ++ip, ++ic;
    }
    // Reached the end of the parent path.
    // Even if the child stops now, it is contained by the parent
    return true;
}

move_results move_file(const fs::path &file, const context &ctx)
{
    move_results results;
    metadata tag{file};
    
    // Does this file have a tag?
    if (!tag.has_tag())
    {
        if (ctx.verbose)
            cerr << "No tag found for file " << file.string()
                 << ".. skipping" << endl;
        return results;
    }
    
    if (ctx.verbose)
    {
        cout << "Properties for " << file.string() << endl;
        tag.print_properties(cout);
    }
    // Format the file, according to either string or script
    auto format = ctx.use_format_script
        ? get_format_from_script(file, tag, ctx)
        : ctx.format;
    auto new_file = format_path_easytag(file, format, tag, ctx);
    
    // See if the new path is actually any different
    if (new_file == file)
    {
        // No change in the file's path.
        if (ctx.verbose)
            cout << "No change in path for " << file.string() << endl;
        return results;
    }

    // Check to see if new path already exists
    if (fs::exists(new_file))
    {
        // Clash with destination path.
        if (ctx.path_uniqueness == path_uniqueness_t::skip)
        {
            cout << "Warning: want to move " << file.string()
                 << " to " << new_file.string()
                 << ", but that path already exists.  Skipping for now.."
                 << endl;
            return results;
        }
        else if (ctx.path_uniqueness == path_uniqueness_t::exit)
        {
            stringstream msg;
            msg << "Tried to move " << file.string()
                << " to " << new_file.string()
                << ", but that path already exists";
            throw path_uniqueness_violation(msg.str());
        }
        else
            throw std::out_of_range("ASSERT: Unknown value of "
                "path_uniqueness_t not handled!");
    }
    
    if (file.parent_path() != new_file.parent_path())
    {
        results.dir_changed = true;
        results.moved_out_of_parent_dir =
            !path_contains(file.parent_path(), new_file.parent_path());
    }
    if (file.filename() != new_file.filename())
        results.filename_changed = true;
    
    if (ctx.simulate || ctx.verbose)
    {
        if (results.dir_changed && results.filename_changed)
            cout << "Move/rename " << file.string()
                 << " to " << new_file.string() << endl;
        else if (results.dir_changed)
            cout << "Move " << file.string()
                 << " to " << new_file.string() << endl;
        else
            cout << "Rename " << file.string()
                 << " to " << new_file.filename().string() << endl;
    }
    
    if (!ctx.simulate)
    {
        // Ensure parent directory path exists before renaming
        fs::create_directories(new_file.parent_path());
        
        // The rename call might fail if the old and new file reside on
        // different devices.  Look out for that situation
        try
        {
            fs::rename(file, new_file);
        }
        catch (fs::filesystem_error &e)
        {
            // Check for cross-device issue
            if (e.code().category() == boost::system::system_category() &&
                e.code().value() == EXDEV /* code 18 */)
            {
                // Copy and remove instead
                fs::copy_file(file, new_file);
                fs::remove(file);
            }
            else
            {
                // It wasn't a cross-device issue, so rethrow
                std::rethrow_exception(std::current_exception());
            }
        }
    }

    return results;
}

} // namespace mm

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

#include <stdexcept>
#include <iostream>

// This is a MOCK implementation of the classes and functions in format.hpp

namespace fs = boost::filesystem;

namespace mm {

using std::string;

string convert_for_filesystem(const string &str, const context &ctx)
{
    // MOCK - return the string unchanged
    return str;
}

fs::path format_path_easytag(const fs::path &file, const string &format,
                             const metadata &tag, const context &ctx)
{
    // MOCK - assume format contains just a base directory for paths
    fs::path p{format};
    
    // MOCK - interpret the filename as a number meaning a pre-defined format
    auto ext = file.extension();
    auto typ = file.stem().string().substr(0, 3);
    if      (typ == "001") p /= "101-AA1-TT1";
    else if (typ == "002") p /= "102-AA1-TT2";
    else if (typ == "003") p /= "201-AA1-TT3";
    else if (typ == "004") p /= "202-AA1-TT4";
    else if (typ == "005") p /= "Alb1/101-AA1-TT1";
    else if (typ == "006") p /= "Alb1/102-AA1-TT2";
    else if (typ == "007") p /= "Alb1/201-AA1-TT3";
    else if (typ == "008") p /= "Alb1/202-AA1-TT4";
    else if (typ == "009") p /= "Alb2/101-AA1-TT1";
    else if (typ == "010") p /= "Alb2/102-AA1-TT2";
    else if (typ == "011") p /= "Alb2/201-AA1-TT3";
    else if (typ == "012") p /= "Alb2/202-AA1-TT4";
    else
        throw std::out_of_range(typ.c_str());
    p += ext;
    return p;
}

string get_format_from_script(const fs::path &file,
                              const metadata &tag, const context &ctx)
{
    // MOCK - return a basic format
    return "%z/%b/%n-%a-%t";
}

} // namespace mm

/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi All Rights Reserved.
** 
** This file is part of Geopter.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
** 
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi                                   
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com                          
**             Date: May 16th, 2021                                                                                          
********************************************************************************/

#ifndef GEOPTER_STRING_TOOL_H
#define GEOPTER_STRING_TOOL_H


#include <string>
#include <vector>

namespace geopter {

/** Utilities for string operation */
class StringTool
{
public:
    /** Split file path into components */
    static void split_path(const std::string& full_path, std::string& folder_path, std::string& file_base, std::string& ext);

    /** Split string with the deliminater */
    static std::vector<std::string> split(const std::string& s, char delim);

    /** Return true if the string starts with the sepecified prefix */
    static bool starts_with(const std::string& s, const std::string& prefix);

    /** Check whether specific string is contained */
    static bool contains(const std::string& s, const std::string& item);
};

}

#endif //GEOPTER_STRING_TOOL_H

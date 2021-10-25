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

//============================================================================
/// \file   utility.cpp
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================

#include <sstream>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <limits>

#include "utility/utility.h"

using namespace geopter;

void Utility::split_path(const std::string &full_path, std::string &folder_path, std::string &file_base, std::string &ext)
{
    file_base = std::filesystem::path(full_path).stem().u8string();
    folder_path = std::filesystem::path(full_path).parent_path().u8string();
    ext = std::filesystem::path(full_path).extension().u8string();
}

std::vector<std::string> Utility::split(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    std::string item;
    for (auto ch: s) {
        if (ch == delim) {
            if (!item.empty()){
                elems.push_back(item);
            }
            item.clear();
        }
        else {
            item += ch;
        }
    }
    if (!item.empty()){
        elems.push_back(item);
    }

    return elems;
}


bool Utility::starts_with(const std::string &s, const std::string &prefix)
{
    auto size = prefix.size();
    if (s.size() < size) {
        return false;
    }
    return std::equal(std::begin(prefix), std::end(prefix), std::begin(s));
}

bool Utility::contains(const std::string &s, const std::string &item)
{
    if(s.find(item) != std::string::npos){
        return true;
    }else{
        return false;
    }
}


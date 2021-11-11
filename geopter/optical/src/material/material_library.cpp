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
/// \file   material_library.cpp
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#include <math.h>

#ifdef _MSC_VER
    #include "msdirent.h"
#else
    #include <dirent.h>
#endif

#include <algorithm>
#include <iostream>
#include <cstdlib>

#include "material/material_library.h"
#include "material/glass_catalog.h"
#include "material/buchdahl_glass.h"

#include "utility/utility.h"

using namespace geopter;

std::shared_ptr<Air> MaterialLibrary::air_;

MaterialLibrary::MaterialLibrary()
{
    air_ = std::make_shared<Air>();
}

MaterialLibrary::~MaterialLibrary()
{
    air_.reset();
    clear();
}

void MaterialLibrary::clear()
{
    if(!catalogs_.empty()){
        for(auto &cat : catalogs_)
        {
            cat.reset();
        }
        catalogs_.clear();
    }

    if( !materials_.empty() ){
        for(auto &m : materials_){
            m.reset();
        }
        materials_.clear();
    }
}

std::shared_ptr<Air> MaterialLibrary::air()
{
    return air_;
}

GlassCatalog* MaterialLibrary::glass_catalog(int i) const
{
    if(i < (int)catalogs_.size()){
        return catalogs_[i].get();
    }else{
        return nullptr;
    }
}

GlassCatalog* MaterialLibrary::glass_catalog(const std::string catname) const
{
    for(auto &cat : catalogs_)
    {
        if(catname == cat->name()){
            return cat.get();
        }
    }
    return nullptr;
}

int MaterialLibrary::catalog_count() const
{
    return catalogs_.size();
}

std::shared_ptr<Material> MaterialLibrary::find(std::string material_name)
{
    if(Utility::contains(material_name, "_")){
        // assume real glass (ex. N-BK7_SCHOTT)
        std::transform(material_name.begin(), material_name.end(), material_name.begin(), ::toupper); // all-uppercase
        std::vector<std::string> product_and_supplier = Utility::split(material_name, '_');
        std::string product_name    = product_and_supplier[0];
        std::string supplier_name = product_and_supplier[1];
        GlassCatalog* catalog = glass_catalog(supplier_name);
        if(catalog){
            return catalog->glass(product_name);
        }else{
            return nullptr;
        }
    }else if(Utility::contains(material_name, ":")){
        // assume abbe glass
        std::vector<std::string> nd_and_vd = Utility::split(material_name, ':');
        double nd = std::stod(nd_and_vd[0]);
        double vd = std::stod(nd_and_vd[1]);
        auto buchdahl_glass = std::make_shared<BuchdahlGlass>(nd, vd);
        materials_.push_back(buchdahl_glass);
        return buchdahl_glass;
    }else{
        // assume real glass name without catalog (ex. n-bk7)
        std::transform(material_name.begin(), material_name.end(), material_name.begin(), ::toupper); // all-uppercase
        for(int ci = 0; ci < (int)catalogs_.size(); ci++){
            GlassCatalog* catalog = catalogs_[ci].get();
            //std::string product_and_supplier = material_name + "_" + catalog->name();
            std::string product_name = material_name;
            if(catalog->glass(product_name)){ //found
                return catalog->glass(product_name);
            }
        }
        return nullptr;
    }
}


bool MaterialLibrary::load_agf_files(const std::vector<std::string>& agf_paths)
{
    if(agf_paths.empty()) {
        return false;
    }

    this->clear();

    std::unique_ptr<GlassCatalog> cat;

    for(auto itr = agf_paths.begin(); itr != agf_paths.end(); itr++) {
        std::string agf_path = *itr;
        cat = std::make_unique<GlassCatalog>();
        if(cat->load_agf(agf_path)){
            catalogs_.push_back(std::move(cat));
        }else{
            cat.reset();
            std::cout << "Failed to open " << agf_path << std::endl;
        }
    }

    return true;

}


/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi All Rights Reserved.
** 
** This file is part of Geopter.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public
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
#include "material/buchdahl_glass.h"

#include "common/string_tool.h"

using namespace geopter;

std::vector< std::unique_ptr<GlassCatalog> > MaterialLibrary::catalogs_;
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

}

std::shared_ptr<Air> MaterialLibrary::GetAir()
{
    return air_;
}

GlassCatalog* MaterialLibrary::GetGlassCatalog(int i)
{
    if(i < (int)catalogs_.size()){
        return catalogs_[i].get();
    }else{
        return nullptr;
    }
}

GlassCatalog* MaterialLibrary::GetGlassCatalog(const std::string& catalog_name)
{
    for(auto &cat : catalogs_)
    {
        if(catalog_name == cat->Name()){
            return cat.get();
        }
    }
    return nullptr;
}

int MaterialLibrary::NumberOfCatalogs() const
{
    return catalogs_.size();
}

std::shared_ptr<Material> MaterialLibrary::Find(std::string material_name)
{
    if(StringTool::Contains(material_name, "_")){
        // assume real glass (ex. N-BK7_SCHOTT)
        std::transform(material_name.begin(), material_name.end(), material_name.begin(), ::toupper); // all-uppercase
        std::vector<std::string> product_and_supplier = StringTool::Split(material_name, '_');
        std::string product_name  = product_and_supplier[0];
        std::string supplier_name = product_and_supplier[1];
        GlassCatalog* catalog = GetGlassCatalog(supplier_name);
        if(catalog){
            return catalog->GetGlass(product_name);
        }else{
            return nullptr;
        }
    }else if(StringTool::Contains(material_name, ":")){
        // assume model glass
        std::vector<std::string> nd_and_vd = StringTool::Split(material_name, ':');
        double nd = std::stod(nd_and_vd[0]);
        double vd = std::stod(nd_and_vd[1]);
        auto buchdahl_glass = std::make_shared<BuchdahlGlass>(nd, vd);
        return buchdahl_glass;
    }else{
        // assume real glass name without catalog (ex. n-bk7)
        std::transform(material_name.begin(), material_name.end(), material_name.begin(), ::toupper); // all-uppercase
        for(int ci = 0; ci < (int)catalogs_.size(); ci++){
            GlassCatalog* catalog = catalogs_[ci].get();
            //std::string product_and_supplier = material_name + "_" + catalog->name();
            std::string product_name = material_name;
            if(catalog->GetGlass(product_name)){ //found
                return catalog->GetGlass(product_name);
            }
        }
        return nullptr;
    }
}


bool MaterialLibrary::LoadAgfFiles(const std::vector<std::string>& agf_paths)
{
    if(agf_paths.empty()) {
        return false;
    }

    this->clear();

    std::unique_ptr<GlassCatalog> cat;

    for(auto itr = agf_paths.begin(); itr != agf_paths.end(); itr++) {
        std::string agf_path = *itr;
        cat = std::make_unique<GlassCatalog>();
        if(cat->LoadAgf(agf_path)){
            catalogs_.push_back(std::move(cat));
        }else{
            cat.reset();
            std::cout << "Failed to open " << agf_path << std::endl;
        }
    }

    return true;

}


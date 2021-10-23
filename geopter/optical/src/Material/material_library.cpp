#include <math.h>

#ifdef _MSC_VER
    #include "msdirent.h"
#else
    #include <dirent.h>
#endif

#include <algorithm>
#include <iostream>
#include <cstdlib>

#include "Material/material.h"
#include "Material/material_library.h"
#include "Material/glass_catalog.h"
#include "Material/buchdahl_glass.h"
#include "Material/glass.h"

#include "Utility/utility.h"

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


#ifdef _MSC_VER
    #include "msdirent.h"
#else
    #include <dirent.h>
#endif

#include <algorithm>
#include <iostream>
#include <cstdlib>

#include "database.h"
#include "file_io.h"
#include "buchdahl_glass.h"


using namespace geopter;

Database::Database() :
    agf_dir_("")
{

}

Database::~Database()
{
    clear();
}

void Database::clear()
{
    if(!catalogs_.empty()){
        for(auto &cat : catalogs_)
        {
            cat.reset();
        }
        catalogs_.clear();
    }
}

std::string Database::agf_dir() const
{
    return agf_dir_;
}

GlassCatalog* Database::glass_catalog(int i) const
{
    if(i < (int)catalogs_.size()){
        return catalogs_[i].get();
    }else{
        return nullptr;
    }
}

GlassCatalog* Database::glass_catalog(const std::string catname) const
{
    for(auto &cat : catalogs_)
    {
        if(catname == cat->name()){
            return cat.get();
        }
    }
    return nullptr;
}

int Database::catalog_count() const
{
    return catalogs_.size();
}

std::shared_ptr<Medium> Database::find(std::string medium_name)
{
    if(FileIO::contains(medium_name, "_")){
        // assume real glass (ex. N-BK7_SCHOTT)
        std::transform(medium_name.begin(), medium_name.end(), medium_name.begin(), ::toupper); // all-uppercase
        std::vector<std::string> glass_and_cat = FileIO::split(medium_name, '_');
        std::string glassname    = glass_and_cat[0];
        std::string catalog_name = glass_and_cat[1];
        auto catalog = glass_catalog(catalog_name);
        if(catalog){
            return catalog->glass(glassname);
        }else{
            return nullptr;
        }
    }else if(FileIO::contains(medium_name, ".")){
        // assume glasscode
        std::vector<std::string> nd_and_vd = FileIO::split(medium_name, '.');
        double nd = std::stod(nd_and_vd[0]) / 10000.0 + 1.0;
        double vd = std::stod(nd_and_vd[1]) / 100.0;
        auto buchdalh_glass = std::make_shared<BuchdahlGlass>(nd, vd);
        return buchdalh_glass;
    }else{
        // assume real glass name without catalog (ex. n-bk7)
        std::transform(medium_name.begin(), medium_name.end(), medium_name.begin(), ::toupper); // all-uppercase
        for(int ci = 0; ci < (int)catalogs_.size(); ci++){
            GlassCatalog* catalog = catalogs_[ci].get();
            if(catalog->glass(medium_name)){ //found
                return catalog->glass(medium_name);
            }
        }
        return nullptr;
    }
}


bool Database::load_all_agf(const std::string agf_dir_path)
{
    DIR* dp;
    dirent* entry;

    dp = opendir(agf_dir_path.c_str());
    if(dp == nullptr) return false;

    clear();

    std::unique_ptr<GlassCatalog> cat;

    while(true){
        entry = readdir(dp);
        if(entry){
            std::string agf_path = agf_dir_path + "/" + entry->d_name;

            cat = std::make_unique<GlassCatalog>();
            if(cat->load_agf(agf_path)){
                catalogs_.push_back(std::move(cat));
            }else{
                cat.reset();
                std::cout << "Failed to open " << agf_path << std::endl;
            }
        }
        else{
            break;
        }
    }

    //create_glass_hash();

    agf_dir_ = agf_dir_path;

    return true;

}

void Database::create_glass_hash()
{
    glass_hash_.clear();

    for(auto &cat : catalogs_){
        int num_glass = cat->glass_count();
        for(int i = 0; i < num_glass; i++){
            std::string display_name = cat->glass(i)->display_name();
            glass_hash_.emplace(display_name, cat->glass(i));
        }
    }
}

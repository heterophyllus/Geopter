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


#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "material/glass_catalog.h"
#include "common/string_tool.h"
#include "spec/spectral_line.h"


using namespace geopter;

GlassCatalog::GlassCatalog() :
    name_("")
{

}

GlassCatalog::~GlassCatalog()
{
    clear();
}

std::string GlassCatalog::name() const
{
    return name_;
}

int GlassCatalog::glass_count() const
{
    return (int)glasses_.size();
}

std::shared_ptr<Glass> GlassCatalog::glass(std::string product_name)
{
    if(!glasses_.empty())
    {
        for(auto &g : glasses_)
        {
            if(g->product_name() == product_name){
                return g;
            }
        }
    }

    return nullptr;
}


std::shared_ptr<Glass> GlassCatalog::glass(int i)
{
    return glasses_[i];
}

void GlassCatalog::clear()
{
    if(glasses_.size() > 0){
        for(auto &g : glasses_){
            g.reset();
        }
        glasses_.clear();
    }
}

bool GlassCatalog::load_agf(std::string agf_path)
{
    std::ifstream ifs(agf_path);
    std::string line_str;

    if(!ifs.is_open()){
        return false;
    }

    clear();

    // set catalog name
    std::filesystem::path p = agf_path;
    name_ = p.stem().u8string();
    std::transform(name_.begin(), name_.end(), name_.begin(), ::toupper); // all-uppercase
    std::string supplier = name_;


    while(getline(ifs,line_str))
    {
        if(StringTool::starts_with(line_str, "NM"))
        {
            //NM <glass name> <dispersion formula #> <MIL#> <N(d)> <V(d)> <Exclude Sub> <status> <melt freq>

            std::vector<std::string> line_parts = StringTool::split(line_str,' ');
            auto glass = std::make_shared<Glass>();
            glasses_.push_back(std::move(glass));
            std::string glassname = line_parts[1];
            std::transform(glassname.begin(), glassname.end(), glassname.begin(), ::toupper);
            glasses_.back()->set_product_name(glassname);
            glasses_.back()->set_supplier(supplier);
            glasses_.back()->set_dispersion_formula(atoi(line_parts[2].c_str()));

        }
        else if(StringTool::starts_with(line_str, "CD"))
        {
            // CD <dispersion coefficients 1 - 10>

            std::vector<std::string> line_parts = StringTool::split(line_str,' ');
            for(int i = 1; i < (int)line_parts.size(); i++){
                double val;
                try {
                    val = std::stod(line_parts[i]);
                }  catch (...) {
                    val = 0.0;
                }
                glasses_.back()->set_dispersion_coefs(i-1,val);
            }
        }
        else if(StringTool::starts_with(line_str, "TD"))
        {
            std::vector<std::string> line_parts = StringTool::split(line_str,' ');
            if(line_parts.size() == 8){
                double D0 = std::stod(line_parts[1]);
                double D1 = std::stod(line_parts[2]);
                double D2 = std::stod(line_parts[3]);
                double E0 = std::stod(line_parts[4]);
                double E1 = std::stod(line_parts[5]);
                double Ltk = std::stod(line_parts[6]);
                double Tref = std::stod(line_parts[7]);
                glasses_.back()->set_thermal_data(D0, D1, D2, E0, E1, Ltk, Tref);
            }
        }
    }

    if(glasses_.empty()){
        return false;
    }

    return true;
}

void GlassCatalog::print()
{
    std::ostringstream oss;
    print(oss);
    std::cout << oss.str() << std::endl;
}


void GlassCatalog::print(std::ostringstream &oss)
{
    const int idx_w = 4;
    const int val_w = 10;
    const int prec  = 4;

    oss << "Catalog: " << name_ << std::endl;
    oss << std::setw(idx_w) << std::right << "Number";
    oss << std::setw(val_w) << std::right << "Name";
    oss << std::setw(val_w) << std::right << "Nd";
    oss << std::setw(val_w) << std::right << "Vd";
    oss << std::endl;

    int num_glasses = glasses_.size();
    for(int i = 0; i < num_glasses; i++)
    {
        std::string name = glasses_[i]->name();
        double n = glasses_[i]->rindex(SpectralLine::d);
        double vd = glasses_[i]->abbe_d();

        oss << std::setw(idx_w) << std::right << i;
        oss << std::setw(val_w) << std::right << std::fixed << name;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << n;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << vd;
        oss << std::endl;
    }
    oss << std::ends;
}

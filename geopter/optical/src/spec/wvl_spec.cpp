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
#include <iomanip>
#include <sstream>
#include <cassert>

#include "spec/wvl_spec.h"

using namespace geopter;

WvlSpec::WvlSpec() :
    reference_index_(0),
    higher_(0.0),
    lower_(0.0)
{
    clear();
}


WvlSpec::~WvlSpec()
{
    if(!wvls_.empty()){
        for(auto &w : wvls_){
            w.reset();
        }
        wvls_.clear();
    }
}


std::vector<double> WvlSpec::get_wavelength_list() const
{
    const int num_wvls = wvls_.size();
    std::vector<double> wvl_list(num_wvls);

    for(int wi =0; wi < num_wvls; wi++){
        wvl_list[wi] = wvls_[wi]->value();
    }

    return wvl_list;
}

std::vector<double> WvlSpec::get_weight_list() const
{
    const int num_wvls = wvls_.size();
    std::vector<double> wvl_list(num_wvls);

    for(int wi =0; wi < num_wvls; wi++){
        wvl_list[wi] = wvls_[wi]->weight();
    }

    return wvl_list;
}

void WvlSpec::add(double wl, double wt, Rgb render_color)
{
    auto w = std::make_unique<Wvl>(wl, wt, render_color);
    wvls_.push_back(std::move(w));
    update();
}

void WvlSpec::remove(int i)
{
    if(i >= (int)wvls_.size() || i < 0){
        return;
    }else{
        wvls_[i].reset();
        auto itr = wvls_.begin() + i;
        wvls_.erase(itr);
    }
    update();
}

void WvlSpec::clear()
{
    if(!wvls_.empty()){
        for(auto &w : wvls_){
            w.reset();
        }
        wvls_.clear();
    }
    reference_index_ = 0;

    higher_ = 0.0;
    lower_ = 0.0;
}

void WvlSpec::update()
{
    assert( !wvls_.empty());

    higher_ = wvls_[0]->value();
    lower_ = wvls_[0]->value();
    for(int i = 0; i < (int)wvls_.size(); i++){
        if(higher_ < wvls_[i]->value()){
            higher_ = wvls_[i]->value();
        }

        if(lower_ > wvls_[i]->value()){
            lower_ = wvls_[i]->value();
        }
    }
}

void WvlSpec::print()
{
    std::ostringstream oss;
    print(oss);
    std::cout << oss.str() << std::endl;
}

void WvlSpec::print(std::ostringstream &oss)
{
    const int idx_w = 4;
    const int val_w = 10;
    const int prec  = 4;


    // header labels
    //oss << "Wavelengths:" << std::endl;
    oss << std::setw(idx_w) << std::right << "W";
    oss << std::setw(val_w) << std::right << "Value";
    oss << std::setw(val_w) << std::right << "Weight";
    oss << std::endl;

    // list up data
    int num_wvls = wvl_count();
    for(int i = 0; i < num_wvls; i++)
    {
        if(i == reference_index_){
            oss << std::setw(idx_w) << std::right << std::to_string(i) + "*";
        }else{
            oss << std::setw(idx_w) << std::right << i;
        }
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << wvl(i)->value();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << wvl(i)->weight();
        oss << std::endl;
    }
    oss << std::endl;

}

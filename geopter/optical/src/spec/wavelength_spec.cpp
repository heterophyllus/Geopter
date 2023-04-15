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

#include "spec/wavelength_spec.h"

using namespace geopter;


WavelengthSpec::WavelengthSpec() :
    reference_index_(0),
    higher_(0.0),
    lower_(0.0),
    max_weight_(1.0)
{
    clear();
}


WavelengthSpec::~WavelengthSpec()
{
    if(!wvls_.empty()){
        for(auto &w : wvls_){
            w.reset();
        }
        wvls_.clear();
    }
}


std::vector<double> WavelengthSpec::GetWavelengthList() const
{
    const int num_wvls = wvls_.size();
    std::vector<double> wvl_list(num_wvls);

    for(int wi =0; wi < num_wvls; wi++){
        wvl_list[wi] = wvls_[wi]->Value();
    }

    return wvl_list;
}

std::vector<double> WavelengthSpec::GetWeightList() const
{
    const int num_wvls = wvls_.size();
    std::vector<double> wvl_list(num_wvls);

    for(int wi =0; wi < num_wvls; wi++){
        wvl_list[wi] = wvls_[wi]->Weight();
    }

    return wvl_list;
}

void WavelengthSpec::AddWavelength(double wl, double wt, Rgb render_color)
{
    auto w = std::make_unique<Wavelength>(wl, wt, render_color);
    wvls_.push_back(std::move(w));
    update();
}

void WavelengthSpec::RemoveWavelength(int i)
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

void WavelengthSpec::clear()
{
    if(!wvls_.empty()){
        for(auto &w : wvls_){
            w.reset();
        }
        wvls_.clear();
    }
    num_wvls_ = 0;

    reference_index_ = 0;

    higher_ = 0.0;
    lower_ = 0.0;
    max_weight_ = 1.0;
}

void WavelengthSpec::update()
{
    assert( !wvls_.empty());

    num_wvls_ = wvls_.size();

    // update higher/lower values, max weight
    higher_ = wvls_[0]->Value();
    lower_  = wvls_[0]->Value();
    max_weight_ = 0.0;

    for(auto &w : wvls_){
        double val = w->Value();
        if(higher_ < val) higher_ = val;
        if(lower_  > val) lower_ = val;

        double wt = w->Weight();
        if(max_weight_ < wt) max_weight_ = wt;
    }
}

void WavelengthSpec::print()
{
    std::ostringstream oss;
    print(oss);
    std::cout << oss.str() << std::endl;
}

void WavelengthSpec::print(std::ostringstream &oss)
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
    for(int i = 0; i < num_wvls_; i++)
    {
        if(i == reference_index_){
            oss << std::setw(idx_w) << std::right << std::to_string(i) + "*";
        }else{
            oss << std::setw(idx_w) << std::right << i;
        }
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << GetWavelength(i)->Value();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << GetWavelength(i)->Weight();
        oss << std::endl;
    }
    oss << std::endl;

}

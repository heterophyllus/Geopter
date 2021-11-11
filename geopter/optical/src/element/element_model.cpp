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
/// \file   element_model.cpp
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================

#include "element/element_model.h"
#include "element/lens.h"
//#include "element/mirror.h"
#include "element/stop.h"
#include "element/dummy_interface.h"
#include "element/air_gap.h"
#include "system/optical_system.h"
#include "assembly/optical_assembly.h"
#include "material/material.h"


using namespace geopter;

ElementModel::ElementModel(OpticalSystem* sys) :
    opt_sys_(sys)
{

}

ElementModel::~ElementModel()
{
    opt_sys_ = nullptr;
}

Element* ElementModel::element(int i) const
{   
    assert( i >= 0);

    if( i < (int)elements_.size()){
        return elements_[i].get();
    }else{
        return nullptr;
    }

}


int ElementModel::element_count() const
{
    return elements_.size();
}

void ElementModel::create()
{
    int num_gaps = opt_sys_->optical_assembly()->gap_count();
    int stop_index = opt_sys_->optical_assembly()->stop_index();

    clear();

    for(int gi = 0; gi < num_gaps; gi++){
        Gap* gap = opt_sys_->optical_assembly()->gap(gi);

        if(gap->material()->name() == "AIR") {
            if(gi == 0){
                // add dummy as Object
                auto dummy = std::make_unique<DummyInterface>(opt_sys_->optical_assembly()->surface(gi));
                elements_.push_back(std::move(dummy));

            }else{
                Gap* gap_before = opt_sys_->optical_assembly()->gap(gi-1);
                if(gap_before->material()->name() == "AIR"){
                    if(gi == stop_index){
                        // add stop
                        auto stop_elem = std::make_unique<Stop>(opt_sys_->optical_assembly()->surface(gi));
                        elements_.push_back(std::move(stop_elem));
                    }else{
                        // add dummy
                        auto dummy = std::make_unique<DummyInterface>(opt_sys_->optical_assembly()->surface(gi));
                        elements_.push_back(std::move(dummy));
                    }
                }
            }
        }
        else{
            // add lens
            Surface* s1 = opt_sys_->optical_assembly()->surface(gi);
            Surface* s2 = opt_sys_->optical_assembly()->surface(gi+1);
            auto lens = std::make_unique<Lens>(s1, s2, gap);
            elements_.push_back(std::move(lens));
        }
    }

}

void ElementModel::clear()
{
    if(!elements_.empty()){
        for(auto &e:elements_){
            e.reset();
        }
        elements_.clear();
    }
}


void ElementModel::update_model()
{
    create();
}

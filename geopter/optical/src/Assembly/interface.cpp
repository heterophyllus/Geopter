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
/// \file   interface.cpp
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#include "assembly/interface.h"
#include "assembly/circular.h"


using namespace geopter;

Interface::Interface() :
    interact_mode_("transmit"),
    semi_diameter_(0.0)
{
    label_ = "";
    lcl_tfrm_.rotation = Eigen::Matrix3d::Identity(3,3);
    lcl_tfrm_.transfer = Eigen::Vector3d::Zero(3);

    gbl_tfrm_.rotation = Eigen::Matrix3d::Identity(3,3);
    gbl_tfrm_.transfer = Eigen::Vector3d::Zero(3);

    profile_ = std::make_unique<Spherical>(0.0);
    
    decenter_ = nullptr;
}

Interface::~Interface()
{
    profile_.reset();

    if(edge_aperture_){
        edge_aperture_.reset();
    }
    if(clear_aperture_){
        clear_aperture_.reset();
    }
    //decenter_.reset();
}



void Interface::set_label(std::string lbl)
{
    label_ = lbl;
}

void Interface::set_local_transform(const Transformation& tfrm)
{
    lcl_tfrm_ = tfrm;
}

void Interface::set_global_transform(const Transformation& tfrm)
{
    gbl_tfrm_ = tfrm;
}


double Interface::max_aperture() const
{
    if(clear_aperture_){
        return clear_aperture_->max_dimension();
    }else{
        return semi_diameter_;
    }

}


void Interface::set_semi_diameter(double sd)
{
    semi_diameter_ = sd;
}


std::string Interface::aperture_shape() const
{
    if(clear_aperture_){
        return clear_aperture_->shape_name();
    }else{
        return "None";
    }
}

void Interface::remove_clear_aperture()
{
    if(clear_aperture_){
        clear_aperture_.reset();
    }
}

bool Interface::point_inside(double x, double y) const
{
    if(clear_aperture_) {
        return clear_aperture_->point_inside(x, y);
    }
    return true;
}

bool Interface::point_inside(const Eigen::Vector2d& pt) const
{
    if(clear_aperture_) {
        return clear_aperture_->point_inside(pt(0), pt(1));
    }
    return true;
}

void Interface::update()
{
    if(decenter_){
        decenter_->update();
    }
}


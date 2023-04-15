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

#include <iostream>
#include <sstream>

#include "assembly/surface.h"

using namespace geopter;

Surface::Surface(std::string lbl)
{
    label_ = lbl;
    interact_mode_ = "Transmit";
    lcl_tfrm_.rotation = Eigen::Matrix3d::Identity(3,3);
    lcl_tfrm_.transfer = Eigen::Vector3d::Zero(3);

    gbl_tfrm_.rotation = Eigen::Matrix3d::Identity(3,3);
    gbl_tfrm_.transfer = Eigen::Vector3d::Zero(3);

    profile_ = SurfaceProfile<Spherical>(0.0);

    solve_ = nullptr;

    decenter_ = nullptr;
}

Surface::Surface(double r)
{
    label_ = "";
    interact_mode_ = "Transmit";

    profile_ = SurfaceProfile<Spherical>(1/r);

    lcl_tfrm_.rotation = Eigen::Matrix3d::Identity(3,3);
    lcl_tfrm_.transfer = Eigen::Vector3d::Zero(3);

    gbl_tfrm_.rotation = Eigen::Matrix3d::Identity(3,3);
    gbl_tfrm_.transfer = Eigen::Vector3d::Zero(3);

    solve_ = nullptr;

    decenter_ = nullptr;
}

Surface::~Surface()
{
    solve_.reset();
}

std::string Surface::ApertureShape() const
{
    return std::visit([](auto ap){ return ap.ShapeName() ;}, clear_aperture_);
}

double Surface::MaxAperture() const
{
    double max_ap = std::visit([](auto ap){ return ap.MaxDimension() ;}, clear_aperture_);
    return std::max(max_ap, semi_diameter_);
}

void Surface::RemoveClearAperture()
{
    clear_aperture_ = Aperture<NoneAperture>();
}


void Surface::Update()
{
    if(decenter_){
        decenter_->update();
    }
}


void Surface::Print()
{
    std::ostringstream oss;
    Print(oss);
    std::cout << oss.str() << std::endl;
}

void Surface::Print(std::ostringstream& oss)
{
    oss << label_ << ": ";
    oss << this->Curvature() << ", ";
    oss << MaxAperture();
}

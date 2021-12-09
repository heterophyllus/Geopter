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
#include <sstream>

#include "assembly/surface.h"
#include "assembly/aperture.h"

#include "sequential/trace_error.h"


using namespace geopter;

Surface::Surface(std::string lbl) : Interface()
{
    label_ = lbl;
    profile_ = std::make_unique<Spherical>();
    semi_diameter_ = 0.0;
}

Surface::Surface(double r) : Interface()
{
    label_ = "";
    profile_ = std::make_unique<Spherical>();
    profile_->set_radius(r);
    semi_diameter_ = 0.0;
}

Surface::~Surface()
{
    profile_.reset();
}


void Surface::intersect(Eigen::Vector3d &pt, double &s, const Eigen::Vector3d& p0, const Eigen::Vector3d& d, double eps, double z_dir)
{
    profile_->intersect(pt,s,p0,d,eps,z_dir);
}

Eigen::Vector3d Surface::normal(Eigen::Vector3d p)
{ 
    return profile_->normal(p);
}

void Surface::print()
{
    std::ostringstream oss;
    print(oss);
    std::cout << oss.str() << std::endl;
}

void Surface::print(std::ostringstream& oss)
{
    oss << label_ << ": ";
    oss << profile_->cv() << ", ";
    oss << max_aperture();
}

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
/// \file   circular.cpp
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#define _USE_MATH_DEFINES
#include <math.h>

#include "assembly/circular.h"

using namespace geopter;

Circular::Circular(double r) : Aperture()
{
    radius_ = r;
    x_dimension_ = radius_;
    y_dimension_ = radius_;
}

Circular::~Circular()
{

}

void Circular::set_radius(double r)
{
    radius_ = r;
}

std::string Circular::shape_name() const
{
    return "Circular";
}

double Circular::max_dimension() const
{
    return radius_;
}

void Circular::set_dimension(double x, double y)
{
    radius_ = sqrt(x*x + y*y);
}

bool Circular::point_inside(double x, double y) const
{
    if( (x*x + y*y) < radius_*radius_ ){
        return true;
    }else{
        return false;
    }
}

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

#include "assembly/aperture.h"

using namespace geopter;

Aperture::Aperture() :
    x_dimension_(0.0),
    y_dimension_(0.0),
    x_offset_(0.0),
    y_offset_(0.0),
    rotation_(0.0)
{
    
}

Aperture::~Aperture()
{

}

double Aperture::x_dimension() const
{
    return x_dimension_;
}

double Aperture::y_dimension() const
{
    return y_dimension_;
}

Eigen::Vector2d Aperture::dimension() const
{
    Eigen::Vector2d dim({x_dimension_, y_dimension_});
    return dim;
}

void Aperture::set_dimension(double x, double y)
{
    x_dimension_ = x;
    y_dimension_ = y;
}

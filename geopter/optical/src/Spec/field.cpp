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
/// \file   field.cpp
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================

#include "spec/field.h"

using namespace geopter;

Field::Field(double x, double y, double wt, Rgb color, double vuy, double vly, double vux, double vlx) :
    x_(x),
    y_(y),
    wt_(wt),
    vux_(vux),
    vuy_(vuy),
    vlx_(vlx),
    vly_(vly),
    render_color_(color)
{
    aim_pt_ = Eigen::Vector2d::Zero(2);
    object_coord_ = Eigen::Vector3d::Zero(3);
}

Field::~Field()
{

}


Eigen::Vector2d Field::apply_vignetting(const Eigen::Vector2d& pupil) const
{
    Eigen::Vector2d vig_pupil = pupil;

    if(pupil(0) < 0.0){ 
        vig_pupil(0) *= (1.0 - vlx_);
    }else{ 
        vig_pupil(0) *= (1.0 - vux_);
    }

    if(pupil(1) < 0.0){ 
        vig_pupil(1) *= (1.0 - vly_);
    }else{  
        vig_pupil(1) *= (1.0 - vuy_);
    }

    return vig_pupil;
}



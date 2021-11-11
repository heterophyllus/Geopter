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
/// \file   transformation.cpp
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================

#include <iostream>
#include "assembly/transformation.h"
#include "assembly/surface.h"

using namespace geopter;

Transformation::Transformation()
{
    rotation = Eigen::Matrix3d::Identity(3,3);
    transfer = Eigen::Vector3d::Zero(3);
}

Transformation::Transformation(const Eigen::Matrix3d& r, const Eigen::Vector3d& t)
{
    rotation = r;
    transfer = t;
}

void Transformation::transform_after_surface(Eigen::Vector3d& before_pt, Eigen::Vector3d& before_dir, const Surface* srf, const Eigen::Vector3d& inc_pt, const Eigen::Vector3d& after_dir)
{
    if(srf->decenter()){
        // get transformation info after surf
        // not implemented yet
        std::cerr << "not implemented: WaveAberration::transform_after_surface()" << std::endl;

        Transformation r_t = srf->decenter()->tform_after_surf();
        Eigen::Matrix3d r = r_t.rotation;
        Eigen::Vector3d t = r_t.transfer;

        Eigen::Matrix3d rt = r_t.rotation.transpose();
        before_pt = rt*(inc_pt - t);
        before_dir = rt*(after_dir);

    }else{
        before_pt = inc_pt;
        before_dir = after_dir;
    }
}

/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
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
/// \file   transformation.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Eigen/Core"

namespace geopter{

class Surface;

struct Transformation
{
    Transformation();
    Transformation(const Eigen::Matrix3d& r, const Eigen::Vector3d& t);

    Eigen::Matrix3d rotation;
    Eigen::Vector3d transfer;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    static void transform_after_surface(Eigen::Vector3d& before_pt, Eigen::Vector3d& before_dir, const Surface* srf, const Eigen::Vector3d& inc_pt, const Eigen::Vector3d& after_dir);
};


} //namespace geopter

#endif // TRANSFORM_H

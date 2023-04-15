/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
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



#ifndef DECENTERDATA_H
#define DECENTERDATA_H

#include "transformation.h"

#include "Eigen/Core"
#include "Eigen/Geometry"


namespace geopter {


enum DecType{
    LOCAL,
    REV,
    DAR,
    BEND
};

class DecenterData
{
    /**
    * @brief Maintains data and actions for position and orientation changes.
    *
        - LOCAL: pos and orientation applied prior to surface
        - REV:   pos and orientation applied following surface in reverse
        - DAR:   pos and orientation applied prior to surface and then returned to initial frame
        - BEND:  used for fold mirrors, orientation applied before and after surface
    */
public:
    DecenterData(int dtype, double x=0.0, double y=0.0, double alpha=0.0, double beta=0.0, double gamma=0.0);
    ~DecenterData();

    Transformation tfrom_before_surf();
    Transformation tform_after_surf();

    void update();

    Eigen::Matrix3d euler2mat(double ai, double aj, double ak);

private:
    int dectype_;

    // vertex decenter
    double x_;
    double y_;
    Eigen::Vector3d dec_;

    // euler angle
    double alpha_;
    double beta_;
    double gamma_;
    Eigen::Vector3d euler_;

    // rotation point offset
    Eigen::Vector3d rot_pt_;
    Eigen::Matrix3d rot_mat_;
};

} //namespace geopter

#endif // DECENTERDATA_H

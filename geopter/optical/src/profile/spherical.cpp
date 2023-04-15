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

#include "profile/spherical.h"
#include "sequential/trace_error.h"

using namespace geopter;

double Spherical::Sag(double x, double y) const
{
    if(fabs(cv_) < std::numeric_limits<double>::epsilon())
    {
        return 0.0;
    }
    else
    {
        double r = 1/cv_;
        double adj2 = r*r - x*x - y*y;

        if(adj2 < 0.0){
            return NAN;
        }

        double adj = sqrt(adj2);

        return r*(1-fabs(adj/r));
    }

}


bool Spherical::Intersect(Eigen::Vector3d &pt, double &distance, const Eigen::Vector3d& p0, const Eigen::Vector3d& dir)
{
    constexpr double z_dir = 1.0; // z direction, currently reflection is not supported

    double ax2 = cv_;
    double cx2 = cv_*(p0.dot(p0)) - 2*p0(2);
    double b = cv_*(dir.dot(p0)) - dir(2);

    double inside_sqrt = b*b - ax2*cx2;

    if(inside_sqrt < 0.0){
        return false;
    }
    else{
        distance = cx2/(z_dir*sqrt(inside_sqrt) -b );
        pt = p0 + distance*dir;
    }

    return true;
}

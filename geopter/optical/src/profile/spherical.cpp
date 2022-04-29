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

#include "profile/spherical.h"
#include "sequential/trace_error.h"

using namespace geopter;

Spherical::Spherical(double c)
{
    name_ = "SPH";
    cv_ = c;
}

Spherical::~Spherical()
{

}

double Spherical::f(const Eigen::Vector3d& p) const
{
    return p(2) - 0.5*cv_*p.dot(p);
}



double Spherical::sag(double x, double y) const
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

        double adj = sqrt(r*r - x*x - y*y);

        return r*(1-fabs(adj/r));
    }

}


bool Spherical::intersect(Eigen::Vector3d &pt, double &s, const Eigen::Vector3d& p0, const Eigen::Vector3d& d, double eps, double z_dir)
{
    Eigen::Vector3d p = p0;
    double ax2 = cv_;
    double cx2 = cv_*(p.dot(p)) - 2*p(2);
    double b = cv_*(d.dot(p)) - d(2);

    double inside_sqrt = b*b - ax2*cx2;

    if(inside_sqrt < 0.0){
        //throw TraceMissedSurfaceError();
        return false;
    }
    else{
        double s1 = cx2/(z_dir*sqrt(b*b - ax2*cx2) -b );
        Eigen::Vector3d p1 = p + s1*d;

        pt = p1;
        s = s1;
    }

    return true;
}

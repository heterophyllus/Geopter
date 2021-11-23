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

#include "profile/surface_profile.h"
#include "sequential/trace_error.h"

using namespace geopter;

SurfaceProfile::SurfaceProfile()
{
    name_ = "None";
    cv_ = 0.0;
}


SurfaceProfile::~SurfaceProfile()
{

}

std::string SurfaceProfile::name() const
{
    return name_;
}

void SurfaceProfile::set_cv(double c)
{
    cv_ = c;
}

double SurfaceProfile::cv() const
{
    return cv_;
}

void SurfaceProfile::set_radius(double r)
{
    if(std::isnan(r)){
        return;
    }else if(std::isinf(r)){
        cv_ = 0.0;
    }else if(fabs(r) < std::numeric_limits<double>::epsilon()){
        cv_ = 0.0;
    }else{
        cv_ = 1.0/r;
    }
}

double SurfaceProfile::radius() const
{
    double r = 0.0;
    if(fabs(cv_) < std::numeric_limits<double>::epsilon()){
        r = std::numeric_limits<double>::infinity();
    }else{
        r = 1.0/cv_;
    }
    return r;
}

double SurfaceProfile::f(const Eigen::Vector3d& p) const
{
    return 0.0;
}

Eigen::Vector3d SurfaceProfile::df(const Eigen::Vector3d& p) const
{
    return Eigen::Vector3d::Zero(3);
}

Eigen::Vector3d SurfaceProfile::normal(const Eigen::Vector3d& p) const
{
    return df(p).normalized();
}

double SurfaceProfile::sag(double x, double y) const
{
    return 0.0;
}

double SurfaceProfile::deriv_1st(double h) const
{
    return 0.0;
}

double SurfaceProfile::deriv_2nd(double h) const
{
    return 0.0;
}

void SurfaceProfile::intersect(Eigen::Vector3d& pt, double& s, const Eigen::Vector3d& p0, const Eigen::Vector3d& d, double eps, double z_dir)
{
    intersect_spencer(pt, s, p0, d, eps, z_dir);
}


void SurfaceProfile::intersect_spencer(Eigen::Vector3d& pt, double& s, const Eigen::Vector3d& p0, const Eigen::Vector3d& d, double eps, double z_dir)
{
    Eigen::Vector3d p = p0;
    double s1 = -f(p)/d.dot(df(p));
    double s2;
    double delta = fabs(s1);
    constexpr int max_itr = 30;
    int iter = 0;

    while(delta > eps && iter < max_itr)
    {
        p = p0 + s1*d;
        s2 = s1 - f(p)/d.dot(df(p));
        delta = fabs(s2-s1);
        s1 = s2;
        iter++;
    }

    pt = p;
    s = s1;
}

void SurfaceProfile::print(std::ostringstream &oss)
{

}

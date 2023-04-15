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

#ifndef SURFACEPROFILE_H
#define SURFACEPROFILE_H

#include <sstream>
#include <vector>
#include "Eigen/Core"
#include "spherical.h"
#include "even_polynomial.h"
#include "odd_polynomial.h"

namespace geopter {

template <class Profile>
class SurfaceProfile : public Profile
{
public:
    SurfaceProfile() : Profile(){}
    SurfaceProfile(double cv) : Profile(cv){}
    SurfaceProfile(double cv, double k, const std::vector<double> coefs) : Profile(cv, k, coefs){}

    void SetCurvature(double c) {
        Profile::cv_ = c;
    }

    /** Returns center curvature */
    double Curvature() const {
        return Profile::cv_;
    }

    void SetRadius(double r){
        if(std::isnan(r)){
            return;
        }else if(std::isinf(r)){
            Profile::cv_ = 0.0;
        }else if(fabs(r) < std::numeric_limits<double>::epsilon()){
            Profile::cv_ = std::numeric_limits<double>::infinity();
        }else{
            Profile::cv_ = 1.0/r;
        }
    }

    /** Returns center radius */
    double Radius() const {
        if(fabs(Profile::cv_) < std::numeric_limits<double>::epsilon()){
            return std::numeric_limits<double>::infinity();
        }else{
            return 1.0/Profile::cv_;
        }
    }

    Eigen::Vector3d Normal(const Eigen::Vector3d& p) const{
        return Profile::df(p).normalized();
    }


};



} //namespace geopter

#endif // SURFACEPROFILE_H

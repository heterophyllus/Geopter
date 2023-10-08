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


#ifndef CIRCULAR_H
#define CIRCULAR_H

#define _USE_MATH_DEFINES
#include <math.h>

#include "aperture/aperture.h"

namespace geopter {


class Circular : public Aperture
{
public:
    Circular(){
        aperture_name_ = "Circular";
        radius_ = 1.0;
    }

    Circular(double r){
        radius_ = r;
    }

    void SetRadius(double r){
        radius_ = r;
    }

    double Radius() const{
        return radius_;
    }

    double MaxDimension() const override{
        return radius_;
    }

    bool PointInside(double x, double y) const override{
        if( (x*x + y*y) <= (radius_*radius_) ){
            return true;
        }else{
            return false;
        }
    }

private:
    double radius_;
};

} //namespace


#endif // CIRCULAR_H

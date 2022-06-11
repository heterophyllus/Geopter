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


#ifndef APERTURE_H
#define APERTURE_H

#include <string>
#include "Eigen/Core"
#include "assembly/none_aperture.h"
#include "assembly/circular.h"

namespace geopter{


/** Apertures on surfaces used to define and limit the light beam passing through a lens system */
template<class Shape>
class Aperture : public Shape
{
public:
    Aperture() : Shape(){}
    Aperture(double r) : Shape(r){}
    Aperture(double x, double y) : Shape(x,y){}

    double x_dimension() const{
        return Shape::x_dimension_;
    }
    double y_dimension() const{
        return Shape::y_dimension_;
    }

    Eigen::Vector2d dimension() const{
        Eigen::Vector2d dim({Shape::x_dimension_, Shape::y_dimension_});
        return dim;
    }

};

} //namespace

#endif // APERTURE_H

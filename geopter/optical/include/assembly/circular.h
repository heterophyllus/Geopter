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

#include <string>

namespace geopter {


class Circular
{
public:
    Circular() : x_dimension_(1.0), y_dimension_(1.0), x_offset_(0.0), y_offset_(0.0), rotation_(0.0){
        radius_ = 1.0;
    }


    Circular(double r) : x_dimension_(r), y_dimension_(r), x_offset_(0.0), y_offset_(0.0), rotation_(0.0){
        radius_ = r;
    }


    Circular(double x, double y) : x_offset_(0.0), y_offset_(0.0), rotation_(0.0){
        radius_ = fabs( std::max(x,y) );
        x_dimension_ = radius_;
        y_dimension_ = radius_;
    }

    void set_radius(double r){
        radius_ = r;
    }

    double radius() const{
        return radius_;
    }

    std::string shape_name() const{
        return "Circular";
    }

    double max_dimension() const{
        return radius_;
    }

    void set_dimension(double x, double y){
        x_dimension_ = x;
        y_dimension_ = y;
        radius_ = sqrt(x*x + y*y);
    }

    bool point_inside(double x, double y) const{
        //constexpr double eps = 1.0e-5;
        if( (x*x + y*y) <= (radius_*radius_) ){
            return true;
        }else{
            return false;
        }
    }


protected:
    double x_dimension_;
    double y_dimension_;
    double x_offset_;
    double y_offset_;
    double rotation_;

private:
    double radius_;
};

} //namespace


#endif // CIRCULAR_H

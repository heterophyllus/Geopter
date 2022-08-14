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


#ifndef PARAXIAL_RAY_SEGMENT_H
#define PARAXIAL_RAY_SEGMENT_H

namespace geopter {


/** Paraxial ray information on a surface */
class ParaxialRaySegment
{
public:
    ParaxialRaySegment(){
        y_ = 0.0;
        u_prime_ = 0.0;
        i_ = 0.0;
        n_prime_ = 0.0;
    }

    ParaxialRaySegment(double y, double u_prime, double i, double n_prime){
        y_ = y;
        u_prime_ = u_prime;
        i_ = i;
        n_prime_ = n_prime;
    }

    ~ParaxialRaySegment(){

    }


    double y() const {
        return y_;
    }

    double u_prime() const {
        return u_prime_;
    }

    double i() const {
        return i_;
    }

    double n_prime() const {
        return n_prime_;
    }

    double l_prime() const{
        return (-y_/u_prime_);
    }

private:
    double y_;
    double u_prime_; // outgoing
    double i_;       // incident
    double n_prime_; // outgoing
};


}

#endif //PARAXIAL_RAY_SEGMENT_H

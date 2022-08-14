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

#ifndef PARAXIALRAY_H
#define PARAXIALRAY_H

#include <vector>
#include <string>
#include <sstream>

#include "paraxial_ray_segment.h"

namespace geopter {


/** Paraxial ray */
class ParaxialRay
{
public:
    ParaxialRay();
    ParaxialRay(int n);
    ~ParaxialRay();

    void prepend(const ParaxialRaySegment& segment);
    void prepend(double y, double u_prime, double i= 0, double n_prime = 1.0);
    
    void append(const ParaxialRaySegment& segment);
    void append(double y, double u_prime, double i= 0, double n_prime = 1.0);

    void clear();

    std::string name() const { return name_; }
    void set_name(std::string name) { name_ = name; }

    /** Return number of components, usually equal to number of surfaces. */
    int number_of_segments() const {
        return (int)prx_ray_segments_.size();
    }

    /** Access to data at the given index */
    ParaxialRaySegment& at(int i) { return prx_ray_segments_[i]; }

    /** Returns the data at the beginning */
    ParaxialRaySegment& front() { return prx_ray_segments_.front();}

    /** Return the data at the last surface */
    ParaxialRaySegment& back() { return prx_ray_segments_.back();}

    /** Access to the data at the surface just before the image */
    ParaxialRaySegment& at_before_image(){
        int img_idx = prx_ray_segments_.size() - 1;
        return prx_ray_segments_[img_idx - 1];
    }

    /** Write ray data to standard output */
    void print() const;

    /** Write property data to stream */
    void print(std::ostringstream& oss) const;

private:
    std::vector< ParaxialRaySegment > prx_ray_segments_;
    std::string name_;
};

} //namespace geopter

#endif // PARAXIALRAY_H

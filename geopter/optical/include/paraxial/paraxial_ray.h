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

//============================================================================
/// \file   paraxial_ray.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#ifndef PARAXIALRAY_H
#define PARAXIALRAY_H

#include <vector>
#include <string>
#include <sstream>

#include "paraxial_ray_at_surface.h"

namespace geopter {


/** Paraxial ray */
class ParaxialRay
{
public:
    ParaxialRay();
    ~ParaxialRay();

    void prepend(std::shared_ptr<ParaxialRayAtSurface> par_ray_at_srf);
    void prepend(double y, double u_prime, double i= 0, double n_prime = 1.0);
    
    void append(std::shared_ptr<ParaxialRayAtSurface> par_ray_at_srf);
    void append(double y, double u_prime, double i= 0, double n_prime = 1.0);

    void clear();

    std::string name() const;
    void set_name(std::string name);

    /** Return number of components, usually equal to number of surfaces. */
    int size() const;

    /** Access to data at the given index */
    std::shared_ptr<ParaxialRayAtSurface> at(int i) const;

    /** Returns the data at the beginning */
    std::shared_ptr<ParaxialRayAtSurface> front() const;

    /** Return the data at the last surface */
    std::shared_ptr<ParaxialRayAtSurface> back() const;

    /** Access to the data at the surface just before the image */
    std::shared_ptr<ParaxialRayAtSurface> at_before_image() const;

    /** Write ray data to standard output */
    void print() const;

    /** Write property data to stream */
    void print(std::ostringstream& oss) const;

private:
    std::vector< std::shared_ptr<ParaxialRayAtSurface> > par_ray_at_srfs_;
    std::string name_;
};

} //namespace geopter

#endif // PARAXIALRAY_H

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
/// \file   aperture.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#ifndef APERTURE_H
#define APERTURE_H

#include <string>
#include "Eigen/Core"

namespace geopter{


/** Apertures on surfaces used to define and limit the light beam passing through a lens system */
class Aperture
{
public:
    enum Shape
    {
        Circular,
        Rectangular
    };

    Aperture();
    virtual ~Aperture();

    virtual double x_dimension() const;
    virtual double y_dimension() const;
    virtual Eigen::Vector2d dimension() const;
    virtual double max_dimension() const =0;
    virtual void set_dimension(double x, double y);

    virtual bool point_inside(double x, double y) const =0;
    virtual std::string shape_name() const =0;

protected:
    double x_dimension_;
    double y_dimension_;
    double x_offset_;
    double y_offset_;
    double rotation_;
};

} //namespace

#endif // APERTURE_H

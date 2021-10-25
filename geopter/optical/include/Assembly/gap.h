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
/// \file   gap.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#ifndef GAP_H
#define GAP_H

#include <memory>
#include <string>

#include "material/material.h"
#include "material/material_library.h"


namespace geopter {


/**
 * @brief Gap container class
 *
 * The gap class represents the space between 2 surfaces. It contains the
    media definition for the space and a (z) displacement between the
    adjacent surfaces.

    The most common use case is an optical system with surfaces centered on a
    common axis. The Gap structure implements this case in the simplest manner.
    More complicated transformations between surfaces are implemented using
    transformations associated with the surfaces themselves.
 */
class Gap
{
public:
    Gap();
    Gap(double t, Material* m);
    ~Gap();

    inline double thi() const;
    inline void set_thi(double t);

    inline Material* material() const;

    void set_material(Material* m);

private:
    double thi_;
    Material* material_;
};


double Gap::thi() const
{
    return thi_;
}

void Gap::set_thi(double t)
{
    thi_ = t;
}


Material* Gap::material() const
{
    return material_;
}


} //namespace geopter

#endif // GAP_H

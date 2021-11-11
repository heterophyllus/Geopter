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
/// \file   material.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>


namespace geopter {

/** Constant refractive index medium */
class Material
{
public:
    Material(double nd=1.0, std::string name="");
    virtual ~Material();

    virtual std::string name() const;
    virtual void set_name(std::string name);

    /** Return refractive index at specified wavelength */
    virtual double rindex(double wv_nm) const;

    /** Returns Abbe number in d-lne */
    virtual double abbe_d() const;

protected:
    std::string name_;
    double n_;
};

} //namespace geopter

#endif // MATERIAL_H

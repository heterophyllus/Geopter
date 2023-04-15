/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
** 
** This file is part of Geopter.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
** 
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
** 
** You should have received a copy of the GNU General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi                                   
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com                          
**             Date: May 16th, 2021                                                                                          
********************************************************************************/

#ifndef AIR_H
#define AIR_H

#include "material.h"

namespace geopter {


class Air : public Material
{
public:
    Air();
    ~Air();

    double RefractiveIndex(double wv_nm) const override;
    double Abbe_d() const override;

    static double RefractiveIndexAbs(double wvl_micron, double T, double P= 101325.0);

private:
    static double RefractiveIndex15deg1atm(double wvl_micron);
};



} // namespace

#endif // AIR_H

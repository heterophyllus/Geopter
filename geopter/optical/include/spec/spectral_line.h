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

#ifndef SPECTRALLINE_H
#define SPECTRALLINE_H

#include <string>

namespace geopter {


/** Constant values for Fraunhofer spectral line */
class SpectralLine
{
public:

    /** get wavelength value from spectral line name */
    static double wavelength(const std::string& spectralname);

    static const double t;
    static const double s;
    static const double r;
    static const double C;
    static const double C_;
    static const double D;
    static const double d;
    static const double e;
    static const double F;
    static const double F_;
    static const double g;
    static const double h;
    static const double i;
};

} //namespace geopter

#endif // SPECTRALLINE_H

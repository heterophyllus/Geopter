/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi All Rights Reserved.
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
**             Date: November 12th, 2021
********************************************************************************/

#ifndef GEOPTER_FUNDAMENTAL_DATA_H
#define GEOPTER_FUNDAMENTAL_DATA_H

namespace geopter{

/** Container for frequently used fundamental data */
struct FundamentalData
{
    int number_of_wavelengths;
    int number_of_fields;
    int reference_wvl_index;
    double reference_wvl_value;

    double enp_distance;
    double enp_radius;
    double exp_distance;
    double exp_radius;

    int number_of_surfaces;
    int image_surface_index;
    double object_distance;
    double image_distance;
};


}

#endif //GEOPTER_FUNDAMENTAL_DATA_H

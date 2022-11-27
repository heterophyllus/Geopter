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
**             Date: November 11th, 2021
********************************************************************************/

#ifndef WAVEFRONT_H
#define WAVEFRONT_H

#include "analysis/wave_aberration.h"
//#include "data/map_data_3d.h"
#include "data/data_grid.h"

namespace geopter{

/** Wavefront
 *
 *  Wavefront is a grid array of optical path differences of the rays that is referenced to the chief ray on the exit pupil
 */
class WavefrontMap : public WaveAberration
{
public:
    WavefrontMap(OpticalSystem *opt_sys);

    /** create by tracing multiple rays */
    std::shared_ptr<DataGrid> create(const Field* fld, double wvl, int ndim);

protected:
    int ndim_;
    double wvl_;
};

} //namespace geopter

#endif //WAVEFRONT_H

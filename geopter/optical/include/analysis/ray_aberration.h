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
** You should have received a copy of the GNU Lesser General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com
**             Date: November 10th, 2021
********************************************************************************/

#ifndef RAY_ABERRATION_H
#define RAY_ABERRATION_H

#include "system/optical_system.h"
#include "data/plot_data.h"

namespace geopter {

class RayAberration
{
public:
    RayAberration(OpticalSystem* opt_sys);
    virtual ~RayAberration();

protected:
    OpticalSystem* opt_sys_;
    int num_fld_;
    int num_wvl_;
    double ref_wvl_val_;
    int ref_wvl_idx_;
};

}

#endif //RAY_ABERRATION_H

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
/// \file   aberration.h
/// \author Hiiragi
/// \date   September 24th, 2021
/// \brief  Declaration of analysis functions
//============================================================================


#ifndef ABERRATION_H
#define ABERRATION_H

#include "system/optical_system.h"
#include "renderer/renderer.h"
#include "data/plot_data.h"

namespace geopter{

/** Analysis regarding ray aberration */
class Aberration
{
public:
    Aberration(OpticalSystem* opt_sys, Renderer* renderer);
    ~Aberration();

    /** @brief Plot transverse ray fan for 1 field
     *  @param nrd number of rays per pupil diameter
     *  @param fld
     *  @param pupil_dir X orY
     *  @param abr_dir DX or DY
    */
    std::shared_ptr<PlotData> plot_transverse_aberration(double nrd, const Field* fld, int pupil_dir= 1, int abr_dir= 1);

    std::shared_ptr<PlotData> plot_longitudinal_spherical_aberration(int num_rays= 10);

    std::shared_ptr<PlotData> plot_astigmatism(int ray_aiming_type, int num_rays= 10);

    std::shared_ptr<PlotData> plot_chromatic_focus_shift(double lower_wvl, double higher_wvl);

    std::shared_ptr<PlotData> plot_spot_diagram(const Field* fld, int pattern, int nrd, double dot_size);

private:
    std::vector<Eigen::Vector2d> create_grid_circle(int nrd);
    std::vector<Eigen::Vector2d> create_hexapolar(int nrd);

    enum SpotRayPattern{
        Grid,
        Hexapolar
    };

    OpticalSystem* opt_sys_;
    Renderer* renderer_;
    int num_fld_;
    int num_wvl_;
    double ref_wvl_val_;
    int ref_wvl_idx_;
};

}


#endif

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
#include <iostream>
#include "analysis/wavefront.h"
#include "sequential/sequential_trace.h"
#include "sequential/trace_error.h"

using namespace geopter;

WavefrontMap::WavefrontMap(OpticalSystem* opt_sys)
    :WaveAberration(opt_sys)
{
    opt_sys_ = opt_sys;
}


std::shared_ptr<DataGrid> WavefrontMap::create(const Field *fld, double wvl, int ndim)
{
    ndim_ = ndim;
    wvl_ = wvl;

    const double nm_to_mm = 1.0e-6;
    const double convert_to_waves = 1.0/(nm_to_mm*wvl);

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);
    tracer->set_aperture_check(true);
    tracer->set_apply_vig(false);

    SequentialPath seq_path = tracer->sequential_path(wvl);

    const auto chief_ray = std::make_shared<Ray>(seq_path.size());
    int trace_result = tracer->trace_pupil_ray(chief_ray, seq_path, Eigen::Vector2d({0.0, 0.0}), fld, wvl);

    const double step = 2.0/static_cast<double>(ndim-1);
    const double start = -1.0;

    Eigen::Vector2d pupil;

    double epd = 2.0*opt_sys_->first_order_data()->enp_radius;
    RayPtr ray = std::make_shared<Ray>(seq_path.size());

    auto data_grid = std::make_shared<DataGrid>(ndim, ndim, epd, epd);

    double cr_exp_dist;
    Eigen::Vector3d cr_exp_pt;
    get_chief_ray_exp_segment(cr_exp_pt, cr_exp_dist, chief_ray);
    auto ref_sphere = setup_reference_sphere(chief_ray,cr_exp_pt);


    for(int i = 0; i < ndim; i++)
    {
        for(int j = 0 ; j < ndim; j++)
        {
            pupil(0) = start + step*static_cast<double>(j);
            pupil(1) = start + step*static_cast<double>(i);

            if(pupil.norm() < 1.0){
                trace_result = tracer->trace_pupil_ray(ray, seq_path, pupil, fld, wvl);
                if(TRACE_SUCCESS == trace_result){
                    double opd = wave_abr_full_calc(ray, chief_ray, fld, ref_sphere);
                    opd *= convert_to_waves;
                    data_grid->set_value_at(i, j, opd);
                }else{
                    data_grid->set_value_at(i, j, NAN);
                }
            }else{
                data_grid->set_value_at(i, j, NAN);
            }

        }

    }

    delete tracer;

    return data_grid;
}




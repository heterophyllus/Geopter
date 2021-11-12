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
**             Date: November 10th, 2021
********************************************************************************/

#include "analysis/opd_fan.h"
#include "sequential/sequential_trace.h"
#include "sequential/trace_error.h"

using namespace geopter;

OpdFan::OpdFan(OpticalSystem* opt_sys) :
    WaveAberration(opt_sys)
{

}

std::shared_ptr<PlotData> OpdFan::plot(Field* fld, int nrd)
{
    auto plot_data = std::make_shared<PlotData>();
    plot_data->set_title("OPD");

    const double ref_wvl_val = opt_sys_->optical_spec()->spectral_region()->reference_wvl();
    const double nm_to_mm = 1.0e-6;
    const double convert_to_waves = 1.0/(nm_to_mm*ref_wvl_val);

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);

    const int num_wvls = opt_sys_->optical_spec()->spectral_region()->wvl_count();

    for(int wi = 0; wi < num_wvls; wi++)
    {
        double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();

        Rgb render_color = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();

        std::vector<double> pupil_data;
        std::vector<double> opd_data;

        Eigen::Vector2d pupil;

        //ChiefRayPkg cr_pkg = get_chief_ray_pkg(fld, wvl);
        //ReferenceSphere ref_sphere = setup_exit_pupil_coords(cr_pkg, fld, wvl);

        //Eigen::Vector2d aim_pt = tracer->aim_chief_ray(fld, wvl);
        //fld->set_aim_pt(aim_pt);
        std::shared_ptr<Ray> chief_ray = tracer->trace_pupil_ray(Eigen::Vector2d({0.0, 0.0}), fld, wvl);

        for(int ri = 0; ri < nrd; ri++)
        {
            pupil(0) = 0.0;
            pupil(1) = -1.0 + (double)ri*2.0/(double)(nrd-1);

            std::shared_ptr<Ray> ray;
            try{
                ray = tracer->trace_pupil_ray(pupil, fld, wvl);
            }catch(TraceError &e){
                ray = e.ray();
                break;
            }

            if(ray->status() == RayStatus::PassThrough){
                //double opd = wave_abr_full_calc(fld, wvl, ray, cr_pkg, ref_sphere);
                double opd = wave_abr_full_calc(ray, chief_ray);
                opd *= convert_to_waves;

                opd_data.push_back(opd);

                pupil_data.push_back(pupil(1));
            }
        }

        auto graph = std::make_shared<Graph2d>(pupil_data, opd_data, render_color);
        graph->set_name(std::to_string(wvl) + "nm");
        plot_data->add_graph(graph);
    }

    delete tracer;

    return plot_data;
}


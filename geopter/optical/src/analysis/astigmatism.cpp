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

#include <iostream>
#include "analysis/astigmatism.h"
#include "sequential/sequential_trace.h"
#include "sequential/trace_error.h"
#include "renderer/renderer.h"

using namespace geopter;

Astigmatism::Astigmatism(OpticalSystem* opt_sys):
    RayAberration(opt_sys)
{

}

std::shared_ptr<PlotData> Astigmatism::plot(int ray_aiming_type, int num_rays)
{
    const int stop_index = opt_sys_->optical_assembly()->stop_index();
    const double h_stop = opt_sys_->optical_assembly()->surface(stop_index)->semi_diameter();
    const int num_srfs = opt_sys_->optical_assembly()->surface_count();

    double maxfld = opt_sys_->optical_spec()->field_of_view()->max_field();

    auto plot_data = std::make_shared<PlotData>();
    plot_data->set_title("Astigmatism");
    plot_data->set_x_axis_label("astigmatism");
    plot_data->set_y_axis_label("field");

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);
    tracer->set_apply_vig(true);
    tracer->set_aperture_check(true);

    // create temporary fields
    Field *tmp_fld;
    std::vector<Field*> tmp_flds;
    std::vector<double> pupil_offsets;
    std::vector<double> ys;

    tmp_flds.reserve(num_rays);
    pupil_offsets.reserve(num_rays);
    ys.reserve(num_rays);

    if(ray_aiming_type == 0){ // chief ray
        for(int fi = 0; fi < num_rays; fi++){
            tmp_fld = new Field;

            double y = maxfld*(double)fi/(double)(num_rays-1);
            ys.push_back(y);
            tmp_fld->set_y(y);

            tmp_flds.push_back(tmp_fld);
            pupil_offsets.push_back(0.0);
        }
    }else{ // intermidiate ray

        auto ray_upper = std::make_shared<Ray>();
        auto ray_lower = std::make_shared<Ray>();

        ray_upper->allocate(num_srfs);
        ray_lower->allocate(num_srfs);

        SequentialPath seq_path = tracer->sequential_path(ref_wvl_val_);

        Eigen::Vector2d aim_pt;

        for(int fi = 0; fi < num_rays; fi++){
            tmp_fld = new Field;

            double y = maxfld*(double)fi/(double)(num_rays-1);
            ys.push_back(y);
            tmp_fld->set_y(y);

            if(tracer->aim_chief_ray(aim_pt, tmp_fld, ref_wvl_val_) ){
                tmp_fld->set_aim_pt(aim_pt);
            }else{
                std::cerr << "Aim chief ray error" << std::endl;
                continue;
            }

            double vuy = tracer->compute_vignetting_factor_for_pupil(Eigen::Vector2d({0.0, 1.0}), *tmp_fld);
            double vly = tracer->compute_vignetting_factor_for_pupil(Eigen::Vector2d({0.0, -1.0}), *tmp_fld);
            tmp_fld->set_vuy(vuy);
            tmp_fld->set_vly(vly);

            tmp_flds.push_back(tmp_fld);

            tracer->trace_pupil_ray(ray_upper, seq_path, Eigen::Vector2d({0.0, 1.0}),  tmp_fld, ref_wvl_val_);
            tracer->trace_pupil_ray(ray_lower, seq_path, Eigen::Vector2d({0.0, -1.0}), tmp_fld, ref_wvl_val_);

            double h_upper = ray_upper->at(stop_index)->y();
            double h_lower = ray_lower->at(stop_index)->y();

            double pupil_offset = (h_upper + h_lower)/2.0/h_stop;
            pupil_offsets.push_back(pupil_offset);
        }
    }


    // trace coddington for all wavelengths
    const int num_wvls = opt_sys_->optical_spec()->spectral_region()->wvl_count();

    for(int wi = 0; wi < num_wvls; wi++){

        double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();
        Rgb color = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();

        std::vector<double> fy;
        std::vector<double> xfo;
        std::vector<double> yfo;

        for(int fi = 0; fi < num_rays; fi++) {
            tmp_fld = tmp_flds[fi];
            double pupil_offset = pupil_offsets[fi];

            fy.push_back(ys[fi]);

            Eigen::Vector2d s_t = tracer->trace_coddington(tmp_fld, wvl, pupil_offset);

            xfo.push_back(s_t(0));
            yfo.push_back(s_t(1));
        }

        auto graph_sag = std::make_shared<Graph2d>(xfo, fy, color,Renderer::LineStyle::Solid);
        graph_sag->set_name(std::to_string(wvl) + "nm - S");

        auto graph_tan = std::make_shared<Graph2d>(yfo, fy, color,Renderer::LineStyle::Dots);
        graph_tan->set_name(std::to_string(wvl) + "nm - T");

        plot_data->add_graph(graph_sag);
        plot_data->add_graph(graph_tan);
    }

    for(auto& fld : tmp_flds){
        delete fld;
    }

    pupil_offsets.clear();
    ys.clear();

    delete tracer;

    return plot_data;
}

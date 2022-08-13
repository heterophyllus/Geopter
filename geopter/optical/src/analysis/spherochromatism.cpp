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
**             Date: November 10th, 2021
********************************************************************************/

#include <iostream>
#include "analysis/spherochromatism.h"
#include "sequential/sequential_trace.h"
#include "sequential/trace_error.h"
#include "paraxial/paraxial_trace.h"

using namespace geopter;

Spherochromatism::Spherochromatism(OpticalSystem* opt_sys) :
    RayAberration(opt_sys)
{

}


std::shared_ptr<PlotData> Spherochromatism::plot(int num_rays)
{
    const Field* fld0 = opt_sys_->optical_spec()->field_of_view()->field(0);

    // collect l_prime for on-axial data
    std::vector<double> l_primes;

    double y0 = opt_sys_->first_order_data()->ref_y0;
    double u0 = opt_sys_->first_order_data()->ref_u0;

    ParaxialTrace *prx_tracer = new ParaxialTrace(opt_sys_);

    for(int wi = 0; wi < num_wvl_; wi++){
        //std::shared_ptr<ParaxialRay> ax_ray = opt_sys_->paraxial_data()->axial_ray(wi);
        double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();
        std::shared_ptr<ParaxialRay> ax_ray = prx_tracer->trace_paraxial_ray_from_object(y0, u0, wvl);
        double l_prime = ax_ray->back()->l_prime();
        l_primes.push_back(l_prime);
    }

    delete prx_tracer;


    // collect zonal data
    SequentialTrace *tracer = new SequentialTrace(opt_sys_);

    Eigen::Vector2d pupil;

    auto plotdata = std::make_shared<PlotData>();
    plotdata->set_title("Longitudinal Spherical Aberration");
    plotdata->set_x_axis_label("Spherical Aberration");
    plotdata->set_y_axis_label("Pupil");
    plotdata->set_xy_reverse(true);

    for(int wi = 0; wi < num_wvl_; wi++){
        double wvl = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->value();
        Rgb color = opt_sys_->optical_spec()->spectral_region()->wvl(wi)->render_color();

        SequentialPath seq_path = tracer->sequential_path(wvl);

        std::vector<double> py;
        std::vector<double> lsa;

        // on axis
        py.push_back(0.0);
        lsa.push_back(l_primes[wi]);

        for(int ri = 1; ri < num_rays; ri++) {
            pupil(0) = 0.0;
            pupil(1) = (double)ri/(double)(num_rays-1);

            auto ray = std::make_shared<Ray>(seq_path.size());

            if(TRACE_SUCCESS != tracer->trace_pupil_ray(ray, seq_path, pupil, fld0, wvl) ){
                std::cerr << "Failed to trace ray: " << "pupil= (" << pupil(0) << "," << pupil(1) << ")" << std::endl;
                continue;
            }

            py.push_back(pupil(1));

            double y = ray->back()->y();
            double M = ray->back()->M();
            double N = ray->back()->N();
            lsa.push_back(-y*N/M);
        }

        /*
        // interpolation
        std::vector<double> spy(30);
        std::vector<double> slsa(30);

        tk::spline spline_lsa;
        spline_lsa.set_points(py, lsa);

        for(int i = 0; i < 30; i++) {
            spy[i] = (double)i/(30.0 - 1.0);
            slsa[i] = spline_lsa(spy[i]);
        }

        auto pointset = std::make_shared<PointSet>(slsa, spy, color);
        */

        auto graph = std::make_shared<Graph2d>(lsa, py, color);
        graph->set_name("W" + std::to_string(wi));

        plotdata->add_graph(graph);
    }

    delete tracer;

    return plotdata;
}

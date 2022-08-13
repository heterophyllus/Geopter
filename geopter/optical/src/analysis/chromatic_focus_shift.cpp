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

#include "analysis/chromatic_focus_shift.h"
#include "paraxial/paraxial_trace.h"
#include "renderer/renderer.h"

using namespace geopter;

ChromaticFocusShift::ChromaticFocusShift(OpticalSystem* opt_sys) :
    RayAberration(opt_sys)
{

}

std::shared_ptr<PlotData> ChromaticFocusShift::plot(double lower_wvl, double higher_wvl)
{
    constexpr int num_data = 100;
    double wvl_step = (higher_wvl-lower_wvl)/(double)(num_data-1);

    double y0 = opt_sys_->first_order_data()->ref_y0;
    double u0 = opt_sys_->first_order_data()->ref_u0;

    ParaxialTrace *tracer = new ParaxialTrace(opt_sys_);

    auto ref_prx_ray = tracer->trace_paraxial_ray_from_object(y0, u0, ref_wvl_val_);
    double ref_l_prime = ref_prx_ray->back()->l_prime();

    std::vector<double> xdata, ydata;
    xdata.reserve(num_data);
    ydata.reserve(num_data);

    for(int i = 0; i < num_data; i++) {
        double wvl = lower_wvl + (double)i*wvl_step;
        auto prx_ray = tracer->trace_paraxial_ray_from_object(y0, u0, wvl);
        double l_prime = prx_ray->back()->l_prime();

        xdata.push_back(wvl);
        ydata.push_back(l_prime - ref_l_prime);
    }

    delete tracer;

    auto graph = std::make_shared<Graph2d>(xdata, ydata, rgb_black, Renderer::LineStyle::Solid);
    graph->set_name("focus shift");

    auto plot_data = std::make_shared<PlotData>();
    plot_data->set_title("Chromatic Focus Shift");
    plot_data->set_x_axis_label("Wvl(nm)");
    plot_data->set_y_axis_label("FocusShift");
    plot_data->add_graph(graph);

    return plot_data;
}

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

#include "analysis/astigmatism.h"

#include <iostream>

#include "sequential/sequential_trace.h"
#include "sequential/trace_error.h"
#include "renderer/renderer.h"

using namespace geopter;

Astigmatism::Astigmatism(OpticalSystem* opt_sys):
    RayAberration(opt_sys)
{

}


std::shared_ptr<PlotData> Astigmatism::plot(int num_rays)
{
    const int num_srfs = opt_sys_->GetOpticalAssembly()->NumberOfSurfaces();
    const int num_wvls = opt_sys_->GetOpticalSpec()->GetWavelengthSpec()->NumberOfWavelengths();
    const double maxfld = opt_sys_->GetOpticalSpec()->GetFieldSpec()->MaxField();

    auto plot_data = std::make_shared<PlotData>();
    plot_data->SetTitle("Astigmatism");
    plot_data->SetXLabel("astigmatism");
    plot_data->SetYLabel("field");
    plot_data->SetXYReverse(true);

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);
    tracer->SetApplyVig(true);
    tracer->SetApertureCheck(true);

    // create temporary fields
    std::vector<Field*> tmp_flds;
    std::vector<double> ys;

    tmp_flds.reserve(num_rays);
    ys.reserve(num_rays);

    SequentialPath seq_path = tracer->CreateSequentialPath(ref_wvl_val_);

    Eigen::Vector2d aim_pt({0.0, 0.0});
    Eigen::Vector3d obj_pt;

    for(int fi = 0; fi < num_rays; fi++){
        Field* tmp_fld = new Field;

        double y = maxfld*(double)fi/(double)(num_rays-1);
        ys.push_back(y);
        tmp_fld->SetY(y);

        if(tracer->AimChiefRay(aim_pt, obj_pt, tmp_fld, ref_wvl_val_) ){
            tmp_fld->SetAimPt(aim_pt);
            //tmp_fld->set_object_pt(obj_pt);
            tmp_flds.push_back(tmp_fld);
        }else{
            std::cerr << "Aim chief ray error" << std::endl;
            delete tmp_fld;
            continue;
        }

    }


    // trace coddington for all wavelengths

    Eigen::Vector2d s_t({0.0, 0.0});

    auto ray = std::make_shared<Ray>();
    ray->Allocate(num_srfs);

    for(int wi = 0; wi < num_wvls; wi++){

        double wvl = opt_sys_->GetOpticalSpec()->GetWavelengthSpec()->GetWavelength(wi)->Value();
        Rgb color = opt_sys_->GetOpticalSpec()->GetWavelengthSpec()->GetWavelength(wi)->RenderColor();

        SequentialPath seq_path = tracer->CreateSequentialPath(wvl);

        std::vector<double> fy;
        std::vector<double> xfo;
        std::vector<double> yfo;

        for(int fi = 0; fi < num_rays; fi++) {
            Field* tmp_fld = tmp_flds[fi];

            if(TRACE_SUCCESS != tracer->TracePupilRay(ray, seq_path, Eigen::Vector2d({0.0, 0.0}), tmp_fld, wvl)){
                std::cerr << "Failed to trace chief ray" << std::endl;
                continue;
            }

            if( ! tracer->TraceCoddington(s_t, ray, seq_path)){
                std::cerr << "Failed to compute coddington" << std::endl;
                continue;
            }

            fy.push_back(ys[fi]);

            xfo.push_back(s_t(0));
            yfo.push_back(s_t(1));
        }

        auto graph_sag = std::make_shared<Graph2d>(xfo, fy, color,Renderer::LineStyle::Solid);
        graph_sag->SetName("W" + std::to_string(wi) + "-Sag");

        auto graph_tan = std::make_shared<Graph2d>(yfo, fy, color,Renderer::LineStyle::Dots);
        graph_tan->SetName("W" + std::to_string(wi)  + "-Tan");

        plot_data->AddGraph(graph_sag);
        plot_data->AddGraph(graph_tan);
    }

    for(auto& fld : tmp_flds){
        delete fld;
    }

    ys.clear();

    delete tracer;

    return plot_data;
}

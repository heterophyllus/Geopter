/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi All Rights Reserved.
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
**             Date: May 16th, 2021                                                                                          
********************************************************************************/

#include <iostream>

#include "analysis/layout.h"

#include "system/optical_system.h"
#include "element/element_model.h"
#include "sequential/sequential_trace.h"
#include "sequential/trace_error.h"

using namespace geopter;

Layout::Layout(OpticalSystem* sys, Renderer* renderer) :
    opt_sys_(sys),
    renderer_(renderer)
{
    
}

Layout::~Layout()
{
    opt_sys_ = nullptr;
    renderer_  = nullptr;
}

void Layout::Update()
{
    int img_idx = opt_sys_->GetOpticalAssembly()->ImageIndex();
    double sum_d = opt_sys_->GetOpticalAssembly()->OverallLength(1, img_idx); // s1..sI
    renderer_->SetXaxisRange(0.0, sum_d);
    renderer_->SetYaxisRange(-1, 1); // set view to the center
    renderer_->SetAspectRatio(1.0);

    renderer_->Update();
}

void Layout::DrawElements()
{
    // elements
    ElementModel *modeler = new ElementModel(opt_sys_);
    modeler->Create();

    int num_elems = modeler->NumberOfElements();

    for(int i = 0; i < num_elems; i++){
        auto e = modeler->GetElement(i);

        if(e->ElementType() == "Lens"){
            DrawLens(dynamic_cast<Lens*>(e), rgb_black);
        }else if(e->ElementType() == "Stop"){
            DrawStop(dynamic_cast<Stop*>(e), rgb_black);
        }
    }

    delete modeler;
}


void Layout::DrawReferenceRays()
{
    int ref_wvl_idx = opt_sys_->GetOpticalSpec()->GetWavelengthSpec()->ReferenceIndex();
    double ref_wvl_val = opt_sys_->GetOpticalSpec()->GetWavelengthSpec()->ReferenceWavelength();
    int num_flds = opt_sys_->GetOpticalSpec()->GetFieldSpec()->NumberOfFields();
    int num_srfs = opt_sys_->GetOpticalAssembly()->NumberOfSurfaces();

    Rgb color;
    std::shared_ptr<Ray> ray;

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);

    SequentialPath seq_path = tracer->CreateSequentialPath(ref_wvl_val);

    auto r1 = std::make_shared<Ray>();
    auto r2 = std::make_shared<Ray>();
    auto r3 = std::make_shared<Ray>();

    r1->Allocate(num_srfs);
    r2->Allocate(num_srfs);
    r3->Allocate(num_srfs);

    for(int fi = 0; fi < num_flds; fi++)
    {
        Field* fld = opt_sys_->GetOpticalSpec()->GetFieldSpec()->GetField(fi);
        color = fld->RenderColor();

        int trace_result = tracer->TracePupilRay(r1, seq_path, Eigen::Vector2d({0.0, 0.0}), fld, ref_wvl_val);
        DrawSingleRay(r1, color);
        if( TRACE_SUCCESS == trace_result){

        }else if (TRACE_TIR_ERROR == trace_result){
            std::cerr << "Total reflection at surface " << r1->GetReachedSurfaceIndex() << ", chief ray on F" << fi << std::endl;
        }


        trace_result = tracer->TracePupilRay(r2, seq_path, Eigen::Vector2d({0.0, 1.0}), fld, ref_wvl_val);
        DrawSingleRay(r2, color);
        if( TRACE_SUCCESS == trace_result){

        }else if (TRACE_TIR_ERROR == trace_result){
            std::cerr << "Total reflection at surface " << r2->GetReachedSurfaceIndex() << ", upper meridional ray on F" << fi << std::endl;
        }


        trace_result = tracer->TracePupilRay(r3, seq_path, Eigen::Vector2d({0.0, -1.0}), fld, ref_wvl_val);
        DrawSingleRay(r3, color);
        if( TRACE_SUCCESS == trace_result){

        }else if (TRACE_TIR_ERROR == trace_result){
            std::cerr << "Total reflection at surface " << r3->GetReachedSurfaceIndex() << ", lower meridional ray on F" << fi << std::endl;
        }

    }

    delete tracer;
}


void Layout::DrawFanRays(int nrd)
{
    double ref_wvl_val = opt_sys_->GetOpticalSpec()->GetWavelengthSpec()->ReferenceWavelength();
    int num_flds = opt_sys_->GetOpticalSpec()->GetFieldSpec()->NumberOfFields();
    int num_srfs = opt_sys_->GetOpticalAssembly()->NumberOfSurfaces();

    Rgb color;
    auto ray = std::make_shared<Ray>();
    ray->Allocate(num_srfs);
    Eigen::Vector2d pupil;
    
    SequentialTrace *tracer = new SequentialTrace(opt_sys_);

    SequentialPath seq_path = tracer->CreateSequentialPath(ref_wvl_val);

    double step = 2.0/(double)(nrd-1);

    for(int fi = 0; fi < num_flds; fi++)
    {
        Field* fld = opt_sys_->GetOpticalSpec()->GetFieldSpec()->GetField(fi);

        color = opt_sys_->GetOpticalSpec()->GetFieldSpec()->GetField(fi)->RenderColor();

        for(int ri = 0; ri < nrd; ri++) {
            pupil(0) = 0.0;
            pupil(1) = -1.0 + (double)ri*step;

            int trace_result = tracer->TracePupilRay(ray, seq_path, pupil, fld, ref_wvl_val);

            DrawSingleRay(ray, color);
        }

    }

    delete tracer;
}

void Layout::DrawLens(Lens* lens, const Rgb& color)
{
    auto s1 = lens->Surface1();
    auto s2 = lens->Surface2();

    double od1 = s1->SemiDiameter();
    double od2 = s2->SemiDiameter();

    double mech_d = std::max(s1->MaxAperture(), s2->MaxAperture());
    double cv1 = s1->Curvature();
    double cv2 = s2->Curvature();

    Eigen::Vector2d edge_pt1, edge_pt2;

    if(cv1 > 0.0 && cv2 < 0.0){
        //biconvex
        DrawSurface(s1, mech_d, color);
        DrawSurface(s2, mech_d, color);

        edge_pt1(0) = s1->Sag(0.0, mech_d) + s1->GlobalTransform().transfer(2);
        edge_pt2(0) = s2->Sag(0.0, mech_d) + s2->GlobalTransform().transfer(2);
        if( std::isnan(edge_pt1(0)) || std::isnan(edge_pt2(0)) ){
            edge_pt1(0) = 0.0;
            edge_pt2(0) = 0.0;
        }

        edge_pt1(1) = mech_d;
        edge_pt2(1) = mech_d;

    }else if(cv1 > 0.0 && cv2 > 0.0){
        // meniscus <

        if((cv1-cv2) > 0.0){ //positive power
            DrawSurface(s1, mech_d,color);
            DrawSurface(s2, mech_d,   color);

            edge_pt1(0) = s1->Sag(0.0, mech_d) + s1->GlobalTransform().transfer(2);
            edge_pt2(0) = s2->Sag(0.0, mech_d) + s2->GlobalTransform().transfer(2);

            if(std::isnan(edge_pt1(0)) || std::isnan(edge_pt2(0))){
                edge_pt1(0) = 0.0;
                edge_pt2(0) = 0.0;
            }

            edge_pt1(1) = mech_d;
            edge_pt2(1) = mech_d;

        }else{ //negative power
            if(od1 > od2){
                DrawSurface(s1, mech_d,color);
                DrawSurface(s2, od2,   color);
                DrawFlat(s2, od2, mech_d, color);

                edge_pt1(0) = s1->Sag(0.0, mech_d) + s1->GlobalTransform().transfer(2);
                edge_pt2(0) = s2->Sag(0.0, od2) + s2->GlobalTransform().transfer(2);

                if( std::isnan(edge_pt1(0)) || std::isnan(edge_pt2(0)) ){
                    edge_pt1(0) = 0.0;
                    edge_pt2(0) = 0.0;
                }

                edge_pt1(1) = mech_d;
                edge_pt2(1) = mech_d;

            }else{
                DrawSurface(s1, od1,color);
                DrawSurface(s2, mech_d,   color);
                DrawFlat(s1, od1, mech_d, color);

                edge_pt1(0) = s1->Sag(0.0, od1) + s1->GlobalTransform().transfer(2);
                edge_pt2(0) = s2->Sag(0.0, mech_d) + s2->GlobalTransform().transfer(2);

                if( std::isnan(edge_pt1(0)) || std::isnan(edge_pt2(0)) ){
                    edge_pt1(0) = 0.0;
                    edge_pt2(0) = 0.0;
                }

                edge_pt1(1) = mech_d;
                edge_pt2(1) = mech_d;

            }
        }

    }else if(cv1 < 0.0 && cv2 < 0.0){
        // meniscus >

        if( (cv1 - cv2) > 0.0 ){ // positive power
            DrawSurface(s1, mech_d,color);
            DrawSurface(s2, mech_d,   color);

            edge_pt1(0) = s1->Sag(0.0, mech_d) + s1->GlobalTransform().transfer(2);
            edge_pt2(0) = s2->Sag(0.0, mech_d) + s2->GlobalTransform().transfer(2);

            if( std::isnan(edge_pt1(0)) || std::isnan(edge_pt2(0)) ){
                edge_pt1(0) = 0.0;
                edge_pt2(0) = 0.0;
            }

            edge_pt1(1) = mech_d;
            edge_pt2(1) = mech_d;

        }else{ // negative power
            if(od1 > od2){
                DrawSurface(s1, mech_d,color);
                DrawSurface(s2, od2,   color);
                DrawFlat(s2, od2, mech_d, color);

                edge_pt1(0) = s1->Sag(0.0, mech_d) + s1->GlobalTransform().transfer(2);
                edge_pt2(0) = s2->Sag(0.0, od2) + s2->GlobalTransform().transfer(2);

                if( std::isnan(edge_pt1(0)) || std::isnan(edge_pt2(0)) ){
                    edge_pt1(0) = 0.0;
                    edge_pt2(0) = 0.0;
                }

                edge_pt1(1) = mech_d;
                edge_pt2(1) = mech_d;

            }else{
                DrawSurface(s1, od1,color);
                DrawSurface(s2, mech_d,   color);
                DrawFlat(s1, od1, mech_d, color);

                edge_pt1(0) = s1->Sag(0.0, od1) + s1->GlobalTransform().transfer(2);
                edge_pt2(0) = s2->Sag(0.0, mech_d) + s2->GlobalTransform().transfer(2);

                if( std::isnan(edge_pt1(0)) || std::isnan(edge_pt2(0)) ){
                    edge_pt1(0) = 0.0;
                    edge_pt2(0) = 0.0;
                }

                edge_pt1(1) = mech_d;
                edge_pt2(1) = mech_d;
            }
        }

    }else{
        // biconcave
        DrawSurface(s1, od1, color);
        DrawSurface(s2, od2, color);
        DrawFlat(s1, od1, mech_d, color);
        DrawFlat(s2, od2, mech_d, color);

        edge_pt1(0) = s1->Sag(0.0, od1) + s1->GlobalTransform().transfer(2);
        edge_pt2(0) = s2->Sag(0.0, od2) + s2->GlobalTransform().transfer(2);

        if( std::isnan(edge_pt1(0)) || std::isnan(edge_pt2(0)) ){
            edge_pt1(0) = 0.0;
            edge_pt2(0) = 0.0;
        }

        edge_pt1(1) = mech_d;
        edge_pt2(1) = mech_d;
    }

    // upper edge
    renderer_->DrawLine(edge_pt1, edge_pt2, color);
    // lower edge
    edge_pt1(1) *= -1.0;
    edge_pt2(1) *= -1.0;
    renderer_->DrawLine(edge_pt1, edge_pt2, color);

}

void Layout::DrawSurface(Surface* srf, double max_y, const Rgb& color)
{
    const int pt_cnt = 1000;
    std::vector<Eigen::Vector2d> pts;
    Eigen::Vector2d pt;

    double y = -max_y;
    double delta = 2*max_y/((double)pt_cnt-1);
    for(int i = 0; i < pt_cnt; i++){
        try{
            pt(0) = srf->Sag(0.0, y);
        }catch(...){
            break;
        }

        pt(0) += srf->GlobalTransform().transfer(2);
        pt(1) = y;

        pts.push_back(pt);
        y += delta;
    }

    renderer_->DrawPolyline(pts,color);
}

void Layout::DrawSingleRay(const std::shared_ptr<Ray>& ray, const Rgb& color)
{
    for(int i = 1; i <= ray->GetReachedSurfaceIndex(); i++)
    {
        Eigen::Vector3d pt_to = ray->GetSegmentAt(i)->IntersectPt();
        Eigen::Vector3d pt_from = ray->GetSegmentAt(i-1)->IntersectPt();

        Surface* cur_srf = opt_sys_->GetOpticalAssembly()->GetSurface(i);
        Surface* prev_srf = opt_sys_->GetOpticalAssembly()->GetSurface(i-1);

        Eigen::Vector2d endpt1;
        endpt1(0) = pt_from(2) + prev_srf->GlobalTransform().transfer(2);
        endpt1(1) = pt_from(1);

        Eigen::Vector2d endpt2;
        endpt2(0) = pt_to(2) + cur_srf->GlobalTransform().transfer(2);
        endpt2(1) = pt_to(1);

        renderer_->DrawLine(endpt1, endpt2,color);
    }

}

void Layout::DrawFlat(Surface* srf, double min_y, double max_y, const Rgb& color)
{
    Eigen::Vector2d from_pt, to_pt;

    // upper
    try{
        from_pt(0) = srf->Sag(0.0, min_y) + srf->GlobalTransform().transfer(2);
        from_pt(1) = min_y;
        to_pt(0) = from_pt(0);
        to_pt(1) = max_y;

        renderer_->DrawLine(from_pt, to_pt, color);
    }catch(...){
        return;
    }



    // lower
    try{
        from_pt(0) = srf->Sag(0.0, -min_y) + srf->GlobalTransform().transfer(2);
        from_pt(1) = -min_y;
        to_pt(0) = from_pt(0);
        to_pt(1) = -max_y;

        renderer_->DrawLine(from_pt, to_pt, color);
    }catch(...){
        return;
    }

}

void Layout::DrawStop(Stop* stop_elem, const Rgb &color)
{
    double stop_ht = stop_elem->ReferenceSurface()->MaxAperture();

    Eigen::Vector2d from_pt, to_pt;

    // upper
    from_pt(0) = stop_elem->ReferenceSurface()->GlobalTransform().transfer(2);
    from_pt(1) = stop_ht;
    to_pt(0) = from_pt(0);
    to_pt(1) = stop_ht*1.2;
    renderer_->DrawLine(from_pt, to_pt, color);

    //lower
    from_pt(0) = stop_elem->ReferenceSurface()->GlobalTransform().transfer(2);
    from_pt(1) = - stop_ht;
    to_pt(0) = from_pt(0);
    to_pt(1) = -stop_ht*1.2;
    renderer_->DrawLine(from_pt, to_pt, color);

}

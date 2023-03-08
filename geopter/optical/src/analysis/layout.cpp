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

void Layout::update()
{
    int img_idx = opt_sys_->optical_assembly()->image_index();
    double sum_d = opt_sys_->optical_assembly()->overall_length(1, img_idx); // s1..sI
    renderer_->set_x_axis_range(0.0, sum_d);
    renderer_->set_y_axis_range(-1, 1); // set view to the center
    renderer_->set_aspect_ratio(1.0);

    renderer_->update();
}

void Layout::draw_elements()
{
    // elements
    ElementModel *modeler = new ElementModel(opt_sys_);
    modeler->create();

    int num_elems = modeler->element_count();

    for(int i = 0; i < num_elems; i++){
        auto e = modeler->element(i);

        if(e->element_type() == "Lens"){
            draw_lens(dynamic_cast<Lens*>(e), rgb_black);
        }else if(e->element_type() == "Stop"){
            draw_stop(dynamic_cast<Stop*>(e), rgb_black);
        }
    }

    delete modeler;
}


void Layout::draw_reference_rays()
{
    int ref_wvl_idx = opt_sys_->optical_spec()->spectral_region()->reference_index();
    double ref_wvl_val = opt_sys_->optical_spec()->spectral_region()->reference_wavelength();
    int num_flds = opt_sys_->optical_spec()->field_of_view()->field_count();
    int num_srfs = opt_sys_->optical_assembly()->surface_count();

    Rgb color;
    std::shared_ptr<Ray> ray;

    SequentialTrace *tracer = new SequentialTrace(opt_sys_);

    SequentialPath seq_path = tracer->sequential_path(ref_wvl_val);

    auto r1 = std::make_shared<Ray>(num_srfs);
    auto r2 = std::make_shared<Ray>(num_srfs);
    auto r3 = std::make_shared<Ray>(num_srfs);

    for(int fi = 0; fi < num_flds; fi++)
    {
        Field* fld = opt_sys_->optical_spec()->field_of_view()->field(fi);
        color = fld->render_color();

        int trace_result = tracer->trace_pupil_ray(r1, seq_path, Eigen::Vector2d({0.0, 0.0}), fld, ref_wvl_val);
        draw_single_ray(r1, color);
        if( TRACE_SUCCESS == trace_result){

        }else if (TRACE_TIR_ERROR == trace_result){
            std::cerr << "Total reflection at surface " << r1->reached_surface() << ", chief ray on F" << fi << std::endl;
        }


        trace_result = tracer->trace_pupil_ray(r2, seq_path, Eigen::Vector2d({0.0, 1.0}), fld, ref_wvl_val);
        draw_single_ray(r2, color);
        if( TRACE_SUCCESS == trace_result){

        }else if (TRACE_TIR_ERROR == trace_result){
            std::cerr << "Total reflection at surface " << r2->reached_surface() << ", upper meridional ray on F" << fi << std::endl;
        }


        trace_result = tracer->trace_pupil_ray(r3, seq_path, Eigen::Vector2d({0.0, -1.0}), fld, ref_wvl_val);
        draw_single_ray(r3, color);
        if( TRACE_SUCCESS == trace_result){

        }else if (TRACE_TIR_ERROR == trace_result){
            std::cerr << "Total reflection at surface " << r3->reached_surface() << ", lower meridional ray on F" << fi << std::endl;
        }

    }

    delete tracer;
}


void Layout::draw_fan_rays(int nrd)
{
    double ref_wvl_val = opt_sys_->optical_spec()->spectral_region()->reference_wavelength();
    int num_flds = opt_sys_->optical_spec()->field_of_view()->field_count();
    int num_srfs = opt_sys_->optical_assembly()->surface_count();

    Rgb color;
    auto ray = std::make_shared<Ray>(num_srfs);
    Eigen::Vector2d pupil;
    
    SequentialTrace *tracer = new SequentialTrace(opt_sys_);

    SequentialPath seq_path = tracer->sequential_path(ref_wvl_val);

    double step = 2.0/(double)(nrd-1);

    for(int fi = 0; fi < num_flds; fi++)
    {
        Field* fld = opt_sys_->optical_spec()->field_of_view()->field(fi);

        color = opt_sys_->optical_spec()->field_of_view()->field(fi)->render_color();

        for(int ri = 0; ri < nrd; ri++) {
            pupil(0) = 0.0;
            pupil(1) = -1.0 + (double)ri*step;

            int trace_result = tracer->trace_pupil_ray(ray, seq_path, pupil, fld, ref_wvl_val);

            draw_single_ray(ray, color);
        }

    }

    delete tracer;
}

void Layout::draw_lens(Lens* lens, const Rgb& color)
{
    auto s1 = lens->surface1();
    auto s2 = lens->surface2();

    double od1 = s1->semi_diameter();
    double od2 = s2->semi_diameter();

    double mech_d = std::max(s1->max_aperture(), s2->max_aperture());
    double cv1 = s1->cv();
    double cv2 = s2->cv();

    Eigen::Vector2d edge_pt1, edge_pt2;

    if(cv1 > 0.0 && cv2 < 0.0){
        //biconvex
        draw_surface(s1, mech_d, color);
        draw_surface(s2, mech_d, color);

        edge_pt1(0) = s1->sag(0.0, mech_d) + s1->global_transform().transfer(2);
        edge_pt2(0) = s2->sag(0.0, mech_d) + s2->global_transform().transfer(2);
        if( std::isnan(edge_pt1(0)) || std::isnan(edge_pt2(0)) ){
            edge_pt1(0) = 0.0;
            edge_pt2(0) = 0.0;
        }

        edge_pt1(1) = mech_d;
        edge_pt2(1) = mech_d;

    }else if(cv1 > 0.0 && cv2 > 0.0){
        // meniscus <

        if((cv1-cv2) > 0.0){ //positive power
            draw_surface(s1, mech_d,color);
            draw_surface(s2, mech_d,   color);

            edge_pt1(0) = s1->sag(0.0, mech_d) + s1->global_transform().transfer(2);
            edge_pt2(0) = s2->sag(0.0, mech_d) + s2->global_transform().transfer(2);

            if(std::isnan(edge_pt1(0)) || std::isnan(edge_pt2(0))){
                edge_pt1(0) = 0.0;
                edge_pt2(0) = 0.0;
            }

            edge_pt1(1) = mech_d;
            edge_pt2(1) = mech_d;

        }else{ //negative power
            if(od1 > od2){
                draw_surface(s1, mech_d,color);
                draw_surface(s2, od2,   color);
                draw_flat(s2, od2, mech_d, color);

                edge_pt1(0) = s1->sag(0.0, mech_d) + s1->global_transform().transfer(2);
                edge_pt2(0) = s2->sag(0.0, od2) + s2->global_transform().transfer(2);

                if( std::isnan(edge_pt1(0)) || std::isnan(edge_pt2(0)) ){
                    edge_pt1(0) = 0.0;
                    edge_pt2(0) = 0.0;
                }

                edge_pt1(1) = mech_d;
                edge_pt2(1) = mech_d;

            }else{
                draw_surface(s1, od1,color);
                draw_surface(s2, mech_d,   color);
                draw_flat(s1, od1, mech_d, color);

                edge_pt1(0) = s1->sag(0.0, od1) + s1->global_transform().transfer(2);
                edge_pt2(0) = s2->sag(0.0, mech_d) + s2->global_transform().transfer(2);

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
            draw_surface(s1, mech_d,color);
            draw_surface(s2, mech_d,   color);

            edge_pt1(0) = s1->sag(0.0, mech_d) + s1->global_transform().transfer(2);
            edge_pt2(0) = s2->sag(0.0, mech_d) + s2->global_transform().transfer(2);

            if( std::isnan(edge_pt1(0)) || std::isnan(edge_pt2(0)) ){
                edge_pt1(0) = 0.0;
                edge_pt2(0) = 0.0;
            }

            edge_pt1(1) = mech_d;
            edge_pt2(1) = mech_d;

        }else{ // negative power
            if(od1 > od2){
                draw_surface(s1, mech_d,color);
                draw_surface(s2, od2,   color);
                draw_flat(s2, od2, mech_d, color);

                edge_pt1(0) = s1->sag(0.0, mech_d) + s1->global_transform().transfer(2);
                edge_pt2(0) = s2->sag(0.0, od2) + s2->global_transform().transfer(2);

                if( std::isnan(edge_pt1(0)) || std::isnan(edge_pt2(0)) ){
                    edge_pt1(0) = 0.0;
                    edge_pt2(0) = 0.0;
                }

                edge_pt1(1) = mech_d;
                edge_pt2(1) = mech_d;

            }else{
                draw_surface(s1, od1,color);
                draw_surface(s2, mech_d,   color);
                draw_flat(s1, od1, mech_d, color);

                edge_pt1(0) = s1->sag(0.0, od1) + s1->global_transform().transfer(2);
                edge_pt2(0) = s2->sag(0.0, mech_d) + s2->global_transform().transfer(2);

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
        draw_surface(s1, od1, color);
        draw_surface(s2, od2, color);
        draw_flat(s1, od1, mech_d, color);
        draw_flat(s2, od2, mech_d, color);

        edge_pt1(0) = s1->sag(0.0, od1) + s1->global_transform().transfer(2);
        edge_pt2(0) = s2->sag(0.0, od2) + s2->global_transform().transfer(2);

        if( std::isnan(edge_pt1(0)) || std::isnan(edge_pt2(0)) ){
            edge_pt1(0) = 0.0;
            edge_pt2(0) = 0.0;
        }

        edge_pt1(1) = mech_d;
        edge_pt2(1) = mech_d;
    }

    // upper edge
    renderer_->draw_line(edge_pt1, edge_pt2, color);
    // lower edge
    edge_pt1(1) *= -1.0;
    edge_pt2(1) *= -1.0;
    renderer_->draw_line(edge_pt1, edge_pt2, color);

}

void Layout::draw_surface(Surface* srf, double max_y, const Rgb& color)
{
    const int pt_cnt = 1000;
    std::vector<Eigen::Vector2d> pts;
    Eigen::Vector2d pt;

    double y = -max_y;
    double delta = 2*max_y/((double)pt_cnt-1);
    for(int i = 0; i < pt_cnt; i++){
        try{
            pt(0) = srf->sag(0.0, y);
        }catch(...){
            break;
        }

        pt(0) += srf->global_transform().transfer(2);
        pt(1) = y;

        pts.push_back(pt);
        y += delta;
    }

    renderer_->draw_polyline(pts,color);
}

void Layout::draw_single_ray(const std::shared_ptr<Ray>& ray, const Rgb& color)
{
    for(int i = 1; i <= ray->reached_surface(); i++)
    {
        Eigen::Vector3d pt_to = ray->at(i)->intersect_pt();
        Eigen::Vector3d pt_from = ray->at(i-1)->intersect_pt();

        Surface* cur_srf = opt_sys_->optical_assembly()->surface(i);
        Surface* prev_srf = opt_sys_->optical_assembly()->surface(i-1);

        Eigen::Vector2d endpt1;
        endpt1(0) = pt_from(2) + prev_srf->global_transform().transfer(2);
        endpt1(1) = pt_from(1);

        Eigen::Vector2d endpt2;
        endpt2(0) = pt_to(2) + cur_srf->global_transform().transfer(2);
        endpt2(1) = pt_to(1);

        renderer_->draw_line(endpt1, endpt2,color);
    }

}

void Layout::draw_flat(Surface* srf, double min_y, double max_y, const Rgb& color)
{
    Eigen::Vector2d from_pt, to_pt;

    // upper
    try{
        from_pt(0) = srf->sag(0.0, min_y) + srf->global_transform().transfer(2);
        from_pt(1) = min_y;
        to_pt(0) = from_pt(0);
        to_pt(1) = max_y;

        renderer_->draw_line(from_pt, to_pt, color);
    }catch(...){
        return;
    }



    // lower
    try{
        from_pt(0) = srf->sag(0.0, -min_y) + srf->global_transform().transfer(2);
        from_pt(1) = -min_y;
        to_pt(0) = from_pt(0);
        to_pt(1) = -max_y;

        renderer_->draw_line(from_pt, to_pt, color);
    }catch(...){
        return;
    }

}

void Layout::draw_stop(Stop* stop_elem, const Rgb &color)
{
    double stop_ht = stop_elem->reference_surface()->max_aperture();

    Eigen::Vector2d from_pt, to_pt;

    // upper
    from_pt(0) = stop_elem->reference_surface()->global_transform().transfer(2);
    from_pt(1) = stop_ht;
    to_pt(0) = from_pt(0);
    to_pt(1) = stop_ht*1.2;
    renderer_->draw_line(from_pt, to_pt, color);

    //lower
    from_pt(0) = stop_elem->reference_surface()->global_transform().transfer(2);
    from_pt(1) = - stop_ht;
    to_pt(0) = from_pt(0);
    to_pt(1) = -stop_ht*1.2;
    renderer_->draw_line(from_pt, to_pt, color);

}


#include "Analysis/layout.h"

#include "System/optical_system.h"

#include "Spec/optical_spec.h"
#include "Spec/field_spec.h"
#include "Spec/field.h"
#include "Spec/wvl_spec.h"
#include "Spec/spectral_line.h"

#include "Element/element_model.h"
#include "Element/element.h"
#include "Element/lens.h"
#include "Element/dummy_interface.h"
#include "Element/stop.h"

#include "Sequential/sequential_model.h"
#include "Sequential/ray.h"
#include "Sequential/sequential_trace.h"

#include "Assembly/optical_assembly.h"
#include "Assembly/surface.h"
#include "Assembly/surface_profile.h"
#include "Assembly/gap.h"

#include "Renderer/renderer.h"


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
    renderer_->set_x_axis_range(0, sum_d);
    //renderer_->set_y_axis_range(-20, 20);
    renderer_->set_aspect_ratio(1.0);

    renderer_->update();
}

void Layout::draw_elements()
{
    // elements
    opt_sys_->elem_model()->create();

    int num_elems = opt_sys_->elem_model()->element_count();

    for(int i = 0; i < num_elems; i++){
        auto e = opt_sys_->elem_model()->element(i);

        if(e->element_type() == "Lens"){
            draw_lens(dynamic_cast<Lens*>(e), rgb_black);
        }else if(e->element_type() == "Stop"){
            draw_stop(dynamic_cast<Stop*>(e), rgb_black);
        }
    }

}


void Layout::draw_reference_rays()
{
    int ref_wvl_idx = opt_sys_->optical_spec()->spectral_region()->reference_index();
    int num_flds = opt_sys_->optical_spec()->field_of_view()->field_count();

    Rgb color;
    Ray ray;
    
    for(int fi = 0; fi < num_flds; fi++)
    {
        color = opt_sys_->optical_spec()->field_of_view()->field(fi)->render_color();

        ray = opt_sys_->sequential_data()->reference_ray(1,fi,ref_wvl_idx);
        draw_single_ray(ray, color);

        ray = opt_sys_->sequential_data()->reference_ray(2,fi,ref_wvl_idx);
        draw_single_ray(ray, color);

        ray = opt_sys_->sequential_data()->reference_ray(3,fi,ref_wvl_idx);
        draw_single_ray(ray, color);
    }
}


void Layout::draw_fan_rays(int nrd)
{
    int ref_wvl_idx = opt_sys_->optical_spec()->spectral_region()->reference_index();
    int num_flds = opt_sys_->optical_spec()->field_of_view()->field_count();

    Rgb color;
    Ray ray;
    Eigen::Vector2d pupil;
    
    SequentialTrace *tracer = new SequentialTrace(opt_sys_);

    double step = 2.0/(double)(nrd-1);

    for(int fi = 0; fi < num_flds; fi++)
    {
        color = opt_sys_->optical_spec()->field_of_view()->field(fi)->render_color();

        for(int ri = 0; ri < nrd; ri++) {
            pupil(0) = 0.0;
            pupil(1) = -1.0 + (double)ri*step;
            ray = tracer->trace_pupil_ray(pupil, fi, ref_wvl_idx);
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
    double cv1 = s1->profile()->cv();
    double cv2 = s2->profile()->cv();

    Eigen::Vector2d edge_pt1, edge_pt2;

    if(cv1 > 0.0 && cv2 < 0.0){
        //biconvex
        draw_surface(s1, mech_d, color);
        draw_surface(s2, mech_d, color);

        edge_pt1(0) = s1->profile()->sag(0.0, mech_d) + s1->global_transform().transfer(2);
        edge_pt1(1) = mech_d;
        edge_pt2(0) = s2->profile()->sag(0.0, mech_d) + s2->global_transform().transfer(2);
        edge_pt2(1) = mech_d;

    }else if(cv1 > 0.0 && cv2 > 0.0){
        // meniscus <

        if((cv1-cv2) > 0.0){ //positive power
            draw_surface(s1, mech_d,color);
            draw_surface(s2, mech_d,   color);

            edge_pt1(0) = s1->profile()->sag(0.0, mech_d) + s1->global_transform().transfer(2);
            edge_pt1(1) = mech_d;
            edge_pt2(0) = s2->profile()->sag(0.0, mech_d) + s2->global_transform().transfer(2);
            edge_pt2(1) = mech_d;

        }else{ //negative power
            if(od1 > od2){
                draw_surface(s1, mech_d,color);
                draw_surface(s2, od2,   color);
                draw_flat(s2, od2, mech_d, color);

                edge_pt1(0) = s1->profile()->sag(0.0, mech_d) + s1->global_transform().transfer(2);
                edge_pt1(1) = mech_d;
                edge_pt2(0) = s2->profile()->sag(0.0, od2) + s2->global_transform().transfer(2);
                edge_pt2(1) = mech_d;

            }else{
                draw_surface(s1, od1,color);
                draw_surface(s2, mech_d,   color);
                draw_flat(s1, od1, mech_d, color);

                edge_pt1(0) = s1->profile()->sag(0.0, od1) + s1->global_transform().transfer(2);
                edge_pt1(1) = mech_d;
                edge_pt2(0) = s2->profile()->sag(0.0, mech_d) + s2->global_transform().transfer(2);
                edge_pt2(1) = mech_d;

            }
        }

    }else if(cv1 < 0.0 && cv2 < 0.0){
        // meniscus >

        if( (cv1 - cv2) > 0.0 ){ // positive power
            draw_surface(s1, mech_d,color);
            draw_surface(s2, mech_d,   color);

            edge_pt1(0) = s1->profile()->sag(0.0, mech_d) + s1->global_transform().transfer(2);
            edge_pt1(1) = mech_d;
            edge_pt2(0) = s2->profile()->sag(0.0, mech_d) + s2->global_transform().transfer(2);
            edge_pt2(1) = mech_d;

        }else{ // negative power
            if(od1 > od2){
                draw_surface(s1, mech_d,color);
                draw_surface(s2, od2,   color);
                draw_flat(s2, od2, mech_d, color);

                edge_pt1(0) = s1->profile()->sag(0.0, mech_d) + s1->global_transform().transfer(2);
                edge_pt1(1) = mech_d;
                edge_pt2(0) = s2->profile()->sag(0.0, od2) + s2->global_transform().transfer(2);
                edge_pt2(1) = mech_d;

            }else{
                draw_surface(s1, od1,color);
                draw_surface(s2, mech_d,   color);
                draw_flat(s1, od1, mech_d, color);

                edge_pt1(0) = s1->profile()->sag(0.0, od1) + s1->global_transform().transfer(2);
                edge_pt1(1) = mech_d;
                edge_pt2(0) = s2->profile()->sag(0.0, mech_d) + s2->global_transform().transfer(2);
                edge_pt2(1) = mech_d;
            }
        }

    }else{
        // biconcave
        draw_surface(s1, od1, color);
        draw_surface(s2, od2, color);
        draw_flat(s1, od1, mech_d, color);
        draw_flat(s2, od2, mech_d, color);

        edge_pt1(0) = s1->profile()->sag(0.0, od1) + s1->global_transform().transfer(2);
        edge_pt1(1) = mech_d;
        edge_pt2(0) = s2->profile()->sag(0.0, od2) + s2->global_transform().transfer(2);
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
    const int pt_cnt = 100;
    std::vector<Eigen::Vector2d> pts;
    Eigen::Vector2d pt;

    double y = -max_y;
    double delta = 2*max_y/((double)pt_cnt-1);
    for(int i = 0; i < pt_cnt; i++){
        pt(0) = srf->profile()->sag(0.0, y);
        pt(0) += srf->global_transform().transfer(2);
        pt(1) = y;

        pts.push_back(pt);
        y += delta;
    }

    renderer_->draw_polyline(pts,color);
}

void Layout::draw_single_ray(const Ray& ray, const Rgb& color)
{
    for(int i = 1; i < ray.size(); i++)
    {
        Eigen::Vector3d pt_to = ray.at(i).intersect_pt;

        Eigen::Vector3d pt_from = ray.at(i-1).intersect_pt;

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
    from_pt(0) = srf->profile()->sag(0.0, min_y) + srf->global_transform().transfer(2);
    from_pt(1) = min_y;
    to_pt(0) = from_pt(0);
    to_pt(1) = max_y;

    renderer_->draw_line(from_pt, to_pt, color);

    // lower
    from_pt(0) = srf->profile()->sag(0.0, -min_y) + srf->global_transform().transfer(2);
    from_pt(1) = -min_y;
    to_pt(0) = from_pt(0);
    to_pt(1) = -max_y;

    renderer_->draw_line(from_pt, to_pt, color);

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

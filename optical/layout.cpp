
#include "layout.h"

#include "optical_model.h"
#include "optical_spec.h"
#include "field_spec.h"
#include "field.h"
#include "wvl_spec.h"
#include "element_model.h"
#include "element.h"
#include "sequential_model.h"
#include "surface.h"
#include "surface_profile.h"
#include "gap.h"
#include "renderer.h"
#include "spectral_line.h"
#include "trace.h"


using namespace geopter;

Layout::Layout(OpticalModel* opt_model, Renderer* renderer) :
    opt_model_(opt_model),
    renderer_(renderer)
{
    opt_model_->elem_model()->elements_from_sequence();
}

Layout::~Layout()
{
    opt_model_ = nullptr;
    renderer_  = nullptr;
}

void Layout::update()
{
    int num_srfs = opt_model_->seq_model()->surface_count();
    double sum_d = opt_model_->seq_model()->length(1, num_srfs);
    renderer_->set_x_axis_range(0, sum_d);
    //renderer_->set_y_axis_range(-20, 20);
    renderer_->set_aspect_ratio(1.0);

    renderer_->update();
}

void Layout::draw_elements()
{
    // elements
    opt_model_->elem_model()->elements_from_sequence();
    int num_elems = opt_model_->elem_model()->element_count();

    Element* e;
    for(int i = 0; i < num_elems; i++){
        e = opt_model_->elem_model()->element(i);

        if(e->label() == "Lens"){
            draw_lens(dynamic_cast<Lens*>(e), rgb_black);
        }else if(e->label() == "Stop"){
            draw_stop(dynamic_cast<DummyInterface*>(e), rgb_black);
        }
    }

}


void Layout::draw_reference_rays()
{
    int num_flds = opt_model_->optical_spec()->field_of_view()->field_count();
    Field* fld;
    Rgb color;
    Ray ray;
    double ref_wvl = opt_model_->optical_spec()->spectral_region()->reference_wvl();

    for(int fi = 0; fi < num_flds; fi++)
    {
        fld = opt_model_->optical_spec()->field_of_view()->field(fi);
        color = fld->render_color();

        ray = Trace::trace_pupil_ray(*opt_model_, Eigen::Vector2d({0.0,0.0}), *fld, ref_wvl);
        draw_single_ray(ray, color);

        ray = Trace::trace_pupil_ray(*opt_model_, Eigen::Vector2d({0.0,1.0}), *fld, ref_wvl);
        draw_single_ray(ray, color);

        ray = Trace::trace_pupil_ray(*opt_model_, Eigen::Vector2d({0.0,-1.0}), *fld, ref_wvl);
        draw_single_ray(ray, color);
    }
}


void Layout::draw_fan_rays(int nrd)
{
    int num_flds = opt_model_->optical_spec()->field_of_view()->field_count();
    Field* fld;
    Rgb color;
    Ray ray;
    Eigen::Vector2d pupil;
    double ref_wvl = opt_model_->optical_spec()->spectral_region()->reference_wvl();
    double step = 2.0/(double)(nrd-1);

    for(int fi = 0; fi < num_flds; fi++)
    {
        fld = opt_model_->optical_spec()->field_of_view()->field(fi);
        color = fld->render_color();

        for(int ri = 0; ri < nrd; ri++) {
            pupil(0) = 0.0;
            pupil(1) = -1.0 + (double)ri*step;
            ray = Trace::trace_pupil_ray(*opt_model_, pupil, *fld, ref_wvl);
            draw_single_ray(ray, color);
        }

    }
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

        Surface* cur_srf = opt_model_->seq_model()->surface(i);
        Surface* prev_srf = opt_model_->seq_model()->surface(i-1);

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

void Layout::draw_stop(DummyInterface *stop_elem, const Rgb &color)
{
    double stop_ht = stop_elem->sd();

    Eigen::Vector2d from_pt, to_pt;

    // upper
    from_pt(0) = stop_elem->reference_interface()->global_transform().transfer(2);
    from_pt(1) = stop_ht;
    to_pt(0) = from_pt(0);
    to_pt(1) = stop_ht*1.2;
    renderer_->draw_line(from_pt, to_pt, color);

    //lower
    from_pt(0) = stop_elem->reference_interface()->global_transform().transfer(2);
    from_pt(1) = - stop_ht;
    to_pt(0) = from_pt(0);
    to_pt(1) = -stop_ht*1.2;
    renderer_->draw_line(from_pt, to_pt, color);

}

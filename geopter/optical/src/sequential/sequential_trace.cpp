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

#define _USE_MATH_DEFINES
#include <cmath>

#include <limits>
#include <iostream>

#include "sequential/sequential_trace.h"
#include "paraxial/paraxial_trace.h"

using namespace geopter;

SequentialTrace::SequentialTrace(OpticalSystem* sys):
    opt_sys_(sys)
{
    do_aperture_check_ = false;
    do_apply_vig_ = true;
}

SequentialTrace::~SequentialTrace()
{
    opt_sys_ = nullptr;
}

void SequentialTrace::pupil_coord_to_obj(Eigen::Vector3d& pt0, Eigen::Vector3d& dir0, const Eigen::Vector2d& pupil_crd, const Field* fld)
{
    Eigen::Vector2d vig_pupil = pupil_crd;

    if(do_apply_vig_){
        vig_pupil = fld->apply_vignetting(pupil_crd);
    }

    double eprad = opt_sys_->first_order_data()->enp_radius;
    Eigen::Vector2d aim_pt = fld->aim_pt();

    double obj_dist = opt_sys_->optical_assembly()->gap(0)->thi();
    double enp_dist = opt_sys_->first_order_data()->enp_dist;

    pt0 = this->get_default_object_pt(fld);

    Eigen::Vector3d pt1;
    pt1(0) = eprad*vig_pupil(0) + aim_pt(0);
    pt1(1) = eprad*vig_pupil(1) + aim_pt(1);
    pt1(2) = obj_dist + enp_dist;

    dir0 = pt1 - pt0;
    dir0.normalize();
}


TraceError SequentialTrace::trace_pupil_ray(RayPtr ray, const SequentialPath &seq_path, const Eigen::Vector2d &pupil_crd, const Field *fld, double wvl)
{
    Eigen::Vector3d pt0;
    Eigen::Vector3d dir0;

    ray->set_pupil_coord(pupil_crd);
    ray->set_wvl(wvl);

    pupil_coord_to_obj(pt0, dir0, pupil_crd, fld);
    return trace_ray_throughout_path(ray, seq_path, pt0, dir0);
}

bool SequentialTrace::trace_reference_rays(std::vector<RayPtr> &ref_rays, const Field *fld, double wvl)
{
    const int num_srfs = opt_sys_->optical_assembly()->surface_count();

    SequentialPath seq_path = sequential_path(wvl);

    ref_rays.clear();

    constexpr int num_rays = 5;
    std::vector<Eigen::Vector2d> pupils(num_rays);
    pupils[0] = Eigen::Vector2d({0.0, 0.0});
    pupils[1] = Eigen::Vector2d({0.0, 1.0});
    pupils[2] = Eigen::Vector2d({0.0, -1.0});
    pupils[3] = Eigen::Vector2d({1.0, 0.0});
    pupils[4] = Eigen::Vector2d({-1.0, 0.0});

    for(int i = 0; i < num_rays; i++){
        auto ray = std::make_shared<Ray>(num_srfs);
        if( TRACE_SUCCESS != trace_pupil_ray(ray, seq_path, pupils[i], fld, wvl)){
            std::cerr << "Reference ray trace error" << std::endl;
            return false;
        }
        ref_rays.push_back(ray);
    }
    return true;
}



TraceError SequentialTrace::trace_ray_throughout_path(RayPtr ray, const SequentialPath &seq_path, const Eigen::Vector3d &pt0, const Eigen::Vector3d &dir0)
{
    const int path_size = seq_path.size();

    if(ray->size() != path_size){
        ray->allocate(path_size);
    }

    Eigen::Vector3d before_pt  = pt0;
    Eigen::Vector3d before_dir = dir0;
    Eigen::Vector3d intersect_pt;
    Eigen::Vector3d after_dir;
    double distance_from_before = 0.0;
    double n_out = seq_path.at(0).rind;
    double n_in  = n_out;
    Transformation transformation_from_before = seq_path.at(0).srf->local_transform();
    //double op_delta = 0.0;
    double opl = 0.0;

    //constexpr double z_dir = 1.0; // used for reflection, not yet implemented


    // first surface
    Eigen::Vector3d srf_normal_1st = seq_path.at(0).srf->normal(pt0);
    ray->at(0)->set_data(pt0, srf_normal_1st, dir0, 0.0, 0.0);


    // trace ray throughout the path till the image
    Eigen::Matrix3d rt;
    Eigen::Vector3d t, rel_before_pt, rel_before_dir, foot_of_perpendicular_pt, srf_normal;
    int cur_srf_idx = 1;

    for(cur_srf_idx = 1; cur_srf_idx < path_size; cur_srf_idx++) {

        rt = transformation_from_before.rotation;
        t  = transformation_from_before.transfer;

        rel_before_pt = rt*(before_pt - t); // relative source point looked from current surface
        rel_before_dir = rt*before_dir;     // relative ray direction looked from current surface

        double dist_from_before_to_perpendicular = -rel_before_pt.dot(rel_before_dir); // distance from previous point to foot of perpendicular
        foot_of_perpendicular_pt = rel_before_pt + dist_from_before_to_perpendicular*rel_before_dir; // foot of perpendicular from the current surface apex to the incident ray line

        Surface* cur_srf = seq_path.at(cur_srf_idx).srf;
        double dist_from_perpendicular_to_intersect_pt; // distance from the foot of perpendicular to the intersect point

        if( ! cur_srf->intersect(intersect_pt, dist_from_perpendicular_to_intersect_pt, foot_of_perpendicular_pt, rel_before_dir) ){
            ray->set_status(RayStatus::MissedSurface);
            ray->set_reached_surface(cur_srf_idx - 1);
            return TRACE_MISSEDSURFACE_ERROR;
        }

        distance_from_before = dist_from_before_to_perpendicular + dist_from_perpendicular_to_intersect_pt; // distance between before and current intersect point

        n_out = seq_path.at(cur_srf_idx).rind;
        srf_normal = cur_srf->normal(intersect_pt); // surface normal at the intersect point
        if( ! bend(after_dir ,before_dir, srf_normal, n_in, n_out) ){
            ray->set_status(RayStatus::TotalReflection);
            ray->set_reached_surface(cur_srf_idx);
            ray->at(cur_srf_idx)->set_data(intersect_pt, srf_normal, after_dir.normalized(),distance_from_before, opl);
            return TRACE_TIR_ERROR;
        }

        opl = n_in * distance_from_before;

        ray->at(cur_srf_idx)->set_data(intersect_pt, srf_normal, after_dir.normalized(),distance_from_before, opl);

        if(do_aperture_check_) {
            if( !cur_srf->point_inside(intersect_pt(0),intersect_pt(1)) ){
                ray->set_status(RayStatus::Blocked);
                ray->set_reached_surface(cur_srf_idx);
                return TRACE_BLOCKED_ERROR;
            }
        }

        before_pt  = intersect_pt;
        before_dir = after_dir;
        n_in       = n_out;
        transformation_from_before = cur_srf->local_transform();
    }

    //op_delta += opl;

    ray->set_status(RayStatus::PassThrough);
    ray->set_reached_surface(path_size-1);

    return TRACE_SUCCESS;
}


bool SequentialTrace::trace_coddington(Eigen::Vector2d &s_t, const std::shared_ptr<Ray> ray, const SequentialPath& path)
{
    /* R. Kingslake, "Lens Design Fundamentals", p292 */

    if( ray->status() != RayStatus::PassThrough ){
        s_t(0) = NAN;
        s_t(1) = NAN;
        return false;
    }

    double s_before, t_before;
    double s_after = 0.0;
    double t_after = 0.0;;
    double n_before;
    double n_after;
    double obl_pwr_s, obl_pwr_t;

    double cosU = 1.0;
    double cosU_prime = 1.0;

    // Opening Equation
    if(std::isinf(path.at(0).d)){
        s_before = std::numeric_limits<double>::infinity();
        t_before = std::numeric_limits<double>::infinity();
    }else{
        Eigen::Vector3d dir0 = ray->at(0)->after_dir();
        double cosUpr = dir0(2);
        //double B = opt_sys_->optical_assembly()->gap(0)->thi();
        //double B = fund_data_.object_distance;
        double B = -path.at(0).d; // object distance
        double Zpr = ray->at(1)->z();

        s_before = (B - Zpr)/cosUpr;
        t_before = s_before;
    }

    n_before = path.at(0).rind;

    int num_srf = ray->size();
    for(int i = 1; i < num_srf-1; i++) {
        n_after = path.at(i).rind;
        double cosI = cos(ray->at(i)->aoi());
        double cosI_prime = cos(ray->at(i)->aor());
        //sinI = sqrt(1.0 - cosI*cosI);
        double sinI = sin(ray->at(i)->aoi());
        //sinI_prime = sinI * n_before/n_after;
        //cosI_prime = sqrt(1.0 - sinI_prime*sinI_prime);

        cosU = ray->at(i-1)->N();
        cosU_prime = ray->at(i)->N();
        double sinU = sqrt(1.0 - cosU*cosU);

        Surface* surf = path.at(i).srf;
        if (surf->is_profile<Spherical>()) {
            double c = surf->cv();
            obl_pwr_s = c*(n_after*cosI_prime - n_before*cosI);
            obl_pwr_t = obl_pwr_s;

        }else{ // aspherical

            double y = ray->at(i)->y();

            if(fabs(y) < std::numeric_limits<double>::epsilon()){
                double cs = surf->cv();
                obl_pwr_s = cs*(n_after*cosI_prime - n_before*cosI);
            }else{
                double sinI_U = sinI*cosU - cosI*sinU;
                double cs = sinI_U/y;
                obl_pwr_s = cs*(n_after*cosI_prime - n_before*cosI);
            }

            double d2z_dy2 = 0.0;
            if(surf->is_profile<EvenPolynomial>()){
                d2z_dy2 = surf->profile<EvenPolynomial>()->deriv_2nd(y);
            }else{
                d2z_dy2 = surf->profile<OddPolynomial>()->deriv_2nd(y);
            }

            double cosI_U = cosI*cosU + sinI*sinU;
            double ct = d2z_dy2 * pow( cosI_U, 3);
            obl_pwr_t = ct*(n_after*cosI_prime - n_before*cosI);
        }


        double z1 = ray->at(i)->z();
        double z2 = ray->at(i+1)->z();
        double d = path.at(i).d;

        double D = (d + z2 - z1)/cosU_prime;
        //double D = ray->at(i+1)->distance_from_before();

        s_after = n_after/(n_before/s_before + obl_pwr_s);
        t_after = n_after*cosI_prime*cosI_prime / ( (n_before*cosI*cosI/t_before) + obl_pwr_t );

        s_before = s_after - D;
        t_before = t_after - D;

        n_before = n_after;

    }


    // Closing Equation
    double img_dist = opt_sys_->optical_assembly()->image_space_gap()->thi();

    double z = ray->at(ray->size()-1-1)->z();

    double zs = s_after*cosU_prime + z - img_dist;
    double zt = t_after*cosU_prime + z - img_dist;

    s_t(0) = zs;
    s_t(1) = zt;

    return true;
}

Eigen::Vector2d SequentialTrace::trace_coddington(const Field *fld, double wvl)
{
    /* R. Kingslake, "Lens Design Fundamentals", p292 */

    Eigen::Vector2d s_t({NAN, NAN});

    SequentialPath path = sequential_path(wvl);

    auto ray = std::make_shared<Ray>(path.size());

    if( TRACE_SUCCESS != trace_pupil_ray(ray, path, Eigen::Vector2d({0.0, 0.0}), fld, wvl)){
        return s_t;
    }

    double s_before, t_before;
    double s_after = 0.0;
    double t_after = 0.0;;
    double n_before;
    double n_after;
    double obl_pwr_s, obl_pwr_t;

    double cosU = 1.0;
    double cosU_prime = 1.0;

    // Opening Equation
    if(std::isinf(path.at(0).d)){
        s_before = std::numeric_limits<double>::infinity();
        t_before = std::numeric_limits<double>::infinity();
    }else{
        auto dir0 = ray->at(0)->after_dir();
        double cosUpr = dir0(2);
        double B = opt_sys_->optical_assembly()->gap(0)->thi();
        double Zpr = ray->at(1)->z();

        s_before = (B - Zpr)/cosUpr;
        t_before = s_before;
    }

    n_before = path.at(0).rind;

    int num_srf = ray->size();
    for(int i = 1; i < num_srf-1; i++) {
        n_after = path.at(i).rind;
        double cosI = cos(ray->at(i)->aoi());
        double cosI_prime = cos(ray->at(i)->aor());
        //sinI = sqrt(1.0 - cosI*cosI);
        double sinI = sin(ray->at(i)->aoi());
        //sinI_prime = sinI * n_before/n_after;
        //cosI_prime = sqrt(1.0 - sinI_prime*sinI_prime);

        cosU = ray->at(i-1)->N();
        cosU_prime = ray->at(i)->N();
        double sinU = sqrt(1.0 - cosU*cosU);

        Surface* surf = path.at(i).srf;
        if(surf->is_profile<Spherical>()) {
            double c = surf->cv();
            obl_pwr_s = c*(n_after*cosI_prime - n_before*cosI);
            obl_pwr_t = obl_pwr_s;

        }else{ // aspherical

            double y = ray->at(i)->y();

            if(fabs(y) < std::numeric_limits<double>::epsilon()){
                double cs = surf->cv();
                obl_pwr_s = cs*(n_after*cosI_prime - n_before*cosI);
            }else{
                double sinI_U = sinI*cosU - cosI*sinU;
                double cs = sinI_U/y;
                obl_pwr_s = cs*(n_after*cosI_prime - n_before*cosI);
            }

            double d2z_dy2 = 0.0;
            if(surf->is_profile<EvenPolynomial>()){
                d2z_dy2 = surf->profile<EvenPolynomial>()->deriv_2nd(y);
            }else{
                d2z_dy2 = surf->profile<OddPolynomial>()->deriv_2nd(y);
            }
            double cosI_U = cosI*cosU + sinI*sinU;
            double ct = d2z_dy2 * pow( cosI_U, 3);
            obl_pwr_t = ct*(n_after*cosI_prime - n_before*cosI);
        }


        double z1 = ray->at(i)->z();
        double z2 = ray->at(i+1)->z();
        double d = path.at(i).d;

        double D = (d + z2 - z1)/cosU_prime;
        //double D = ray->at(i+1)->distance_from_before();

        s_after = n_after/(n_before/s_before + obl_pwr_s);
        t_after = n_after*cosI_prime*cosI_prime / ( (n_before*cosI*cosI/t_before) + obl_pwr_t );

        s_before = s_after - D;
        t_before = t_after - D;

        n_before = n_after;

    }


    // Closing Equation
    double img_dist = opt_sys_->optical_assembly()->image_space_gap()->thi();

    double z = ray->at(ray->size()-1-1)->z();

    double zs = s_after*cosU_prime + z - img_dist;
    double zt = t_after*cosU_prime + z - img_dist;

    s_t(0) = zs;
    s_t(1) = zt;

    return s_t;
}


SequentialPath SequentialTrace::sequential_path(double wvl)
{
    const int img = opt_sys_->optical_assembly()->image_index();
    return sequential_path(0, img, wvl);
}

SequentialPath SequentialTrace::sequential_path(int start, int end, double wvl)
{
    const int img = opt_sys_->optical_assembly()->image_index();

    assert(end <= img);

    SequentialPath path;
    SequentialPathComponent path_comp;

    const int num_srf = OpticalAssembly::number_of_surfaces();
    const int num_gap = num_srf -1;

    for(int i = start; i <= end; i++)
    {
        if ( i < num_srf ) {
            path_comp.srf = opt_sys_->optical_assembly()->surface(i);
        }else{
            path_comp.srf = nullptr;
        }

        if( i < num_gap ) {
            path_comp.d = opt_sys_->optical_assembly()->gap(i)->thi();
            path_comp.rind = opt_sys_->optical_assembly()->gap(i)->material()->rindex(wvl);
        }else {
            path_comp.d = 0.0;
            path_comp.rind = 1.0;
        }

        path.append(path_comp);
    }


    return path;
}


bool SequentialTrace::aim_chief_ray(Eigen::Vector2d& aim_pt, Eigen::Vector3d& obj_pt, const Field *fld, double wvl)
{
    int stop = opt_sys_->optical_assembly()->stop_index();
    Eigen::Vector2d xy_target({0.0, 0.0});

    SequentialPath seq_path = sequential_path(wvl);

    auto ray = std::make_shared<Ray>(seq_path.size());
    bool result = search_ray_aiming_at_surface(ray, aim_pt, fld, stop, xy_target);

    if(result){
        obj_pt = ray->at(0)->intersect_pt();
        return true;
    }else{
        return false;
    }

    //return search_aim_point(aim_pt, stop, xy_target, fld, wvl);
}



bool SequentialTrace::search_ray_aiming_at_surface(RayPtr ray, Eigen::Vector2d& aim_pt, const Field *fld, int target_srf_idx, const Eigen::Vector2d &xy_target)
{
    //const int fld_type = opt_sys_->optical_spec()->field_of_view()->field_type();
    double ref_wvl = opt_sys_->optical_spec()->spectral_region()->reference_wvl();

    double obj_dist = opt_sys_->optical_assembly()->gap(0)->thi();
    double enp_dist = opt_sys_->first_order_data()->enp_dist;

    SequentialPath seq_path = sequential_path(ref_wvl);

    Eigen::Vector3d pt0 = this->get_default_object_pt(fld);
    Eigen::Vector3d pt1;
    Eigen::Vector3d dir0;

    aim_pt(0) = 0.0;
    aim_pt(1) = 0.0;

    double y_target = xy_target(1);
    double y_ray1, y_ray2;
    double y1, y2;

    // newton
    int loop = 0;
    constexpr int max_loop_cnt = 30;
    constexpr double delta = 1.0e-5;
    constexpr double error = 1.0e-5;
    int trace_result1, trace_result2;

    ray->allocate(seq_path.size());

    y1 = 0.0;

    while(true){
        loop++;

        if(loop > max_loop_cnt){
            aim_pt(0) = 0.0;
            aim_pt(1) = 0.0;
            return false;
        }


        y2 = y1 + delta;

        aim_pt(0) = 0.0;
        aim_pt(1) = y1;

        pt1(0) = aim_pt(0);
        pt1(1) = aim_pt(1);
        pt1(2) = obj_dist + enp_dist;
        dir0 = pt1 - pt0;
        dir0.normalize();

        ray->set_pupil_coord(aim_pt);
        trace_result1 = trace_ray_throughout_path(ray, seq_path, pt0, dir0);
        if(trace_result1 == TRACE_SUCCESS){
            y_ray1 = ray->at(target_srf_idx)->y();
        }else{
            if(ray->reached_surface() >= target_srf_idx){
                y_ray1 = ray->at(target_srf_idx)->y();
            }else{
                continue;
            }
        }


        aim_pt(0) = 0.0;
        aim_pt(1) = y2;

        pt1(0) = aim_pt(0);
        pt1(1) = aim_pt(1);
        pt1(2) = obj_dist + enp_dist;
        dir0 = pt1 - pt0;
        dir0.normalize();

        ray->set_pupil_coord(aim_pt);
        trace_result2 = trace_ray_throughout_path(ray, seq_path, pt0, dir0);
        if(trace_result2 == TRACE_SUCCESS){
            y_ray2 = ray->at(target_srf_idx)->y();
        }else{
            if(ray->reached_surface() >= target_srf_idx){
                y_ray2 = ray->at(target_srf_idx)->y();
            }else{
                continue;
            }
        }


        if( fabs(y_ray1 - y_target) < error){
            aim_pt(0) = 0.0;
            aim_pt(1) = y1;
            return true;
        }


        double diff_y = (y_ray2 - y_ray1)/(y2 - y1);
        double next_y = y1 - (y_ray1 - y_target)/diff_y;

        y1 = next_y;
    }

    aim_pt(0) = 0.0;
    aim_pt(1) = y1;

    return true;

}

bool SequentialTrace::bend(Eigen::Vector3d& d_out, const Eigen::Vector3d& d_in, const Eigen::Vector3d& normal, double n_in, double n_out)
{
    double normal_len = normal.norm();
    double cosI = d_in.dot(normal)/normal_len;
    double sinI_sqr = 1.0 - cosI*cosI;

    double inside_sqrt = n_out*n_out - n_in*n_in*sinI_sqr;
    if(inside_sqrt < 0.0){
        return false;
    }

    double cosI_sgn = (cosI > 0.0) - (cosI < 0.0);
    //double n_cosIp = sqrt(n_out*n_out - n_in*n_in*sinI_sqr) * cosI_sgn;
    double n_cosIp = sqrt( inside_sqrt ) * cosI_sgn;
    double alpha = n_cosIp - n_in*cosI;
    d_out = (n_in*d_in + alpha*normal)/n_out;

    return true;
}

Eigen::Vector3d SequentialTrace::get_default_object_pt(const Field* fld)
{
    Eigen::Vector3d obj_pt;
    Eigen::Vector3d ang_dg;
    Eigen::Vector3d img_pt;
    Eigen::Vector3d dir_tan;

    auto fod = opt_sys_->first_order_data();

    int field_type = opt_sys_->optical_spec()->field_of_view()->field_type();
    double fld_x = fld->x();
    double fld_y = fld->y();

    switch (field_type)
    {
    case FieldType::OBJ_ANG:
        ang_dg = Eigen::Vector3d({fld_x, fld_y, 0.0});
        dir_tan(0) = tan(ang_dg(0) * M_PI/180.0);
        dir_tan(1) = tan(ang_dg(1) * M_PI/180.0);
        dir_tan(2) = tan(ang_dg(2) * M_PI/180.0);
        obj_pt = -dir_tan*(fod->obj_dist + fod->enp_dist);
        break;

    case FieldType::OBJ_HT:
        obj_pt(0) = fld_x;
        obj_pt(1) = fld_y;
        obj_pt(2) = 0.0;
        break;

    case FieldType::IMG_HT:
        img_pt = Eigen::Vector3d({fld_x, fld_y, 0.0});
        obj_pt = fod->red*img_pt;
        break;

    default:
        obj_pt = Eigen::Vector3d::Zero(3);
    }

    return obj_pt;
}


std::vector<double> SequentialTrace::compute_vignetting_factors(const Field& fld)
{
    double vuy = fld.vuy();
    double vly = fld.vly();
    double vux = fld.vux();
    double vlx = fld.vlx();

    std::vector<double> vig_factors = std::vector<double>( {vuy, vly, vux, vlx} );

    std::vector<Eigen::Vector2d> pupils({Eigen::Vector2d({0.0, 1.0}),
                                         Eigen::Vector2d({0.0, -1.0}),
                                         Eigen::Vector2d({1.0, 0.0}),
                                         Eigen::Vector2d({-1.0, 0.0})});

    for(int i = 0; i < (int)pupils.size(); i++)
    {
        double vig;
        try{
            vig = compute_vignetting_factor_for_pupil(pupils[i], fld);
        }catch(TraceError &e){
            std::cerr << "Could not compute vuy" << pupils[i] << std::endl;
            // not update value
            continue;
        }

        vig_factors[i] = vig;
    }

    return vig_factors;
}


double SequentialTrace::compute_vignetting_factor_for_pupil(const Eigen::Vector2d& full_pupil, const Field &fld)
{
    // save current setting
    bool orig_vig_state = do_apply_vig_;
    do_apply_vig_ = false;

    bool orig_aperture_check = do_aperture_check_;
    do_aperture_check_ = true;

    constexpr double eps = 1.0e-5;
    constexpr int max_loop_cnt = 30;

    const int stop_index = opt_sys_->optical_assembly()->stop_index();
    const double stop_radius = opt_sys_->optical_assembly()->surface(stop_index)->max_aperture();

    const double ref_wvl = opt_sys_->optical_spec()->spectral_region()->reference_wvl();
    SequentialPath path = sequential_path(ref_wvl);

    Eigen::Vector2d vig_pupil = full_pupil;

    double vig = 0.0;

    double a = 0.0;
    double b = 1.0;

    vig_pupil(0) = full_pupil(0)*(1.0 - a);
    vig_pupil(1) = full_pupil(1)*(1.0 - a);
    std::shared_ptr<Ray> ray_full_marginal = std::make_shared<Ray>(path.size());
    int trace_result = trace_pupil_ray(ray_full_marginal, path, vig_pupil, &fld, ref_wvl);

    if(TRACE_BLOCKED_ERROR == trace_result){

    }else if(TRACE_SUCCESS != trace_result){
        if(ray_full_marginal->reached_surface() <= stop_index){
            return NAN;
        }
    }


    if(ray_full_marginal->status() == RayStatus::PassThrough){
        double ray_height_at_stop = ray_full_marginal->at(stop_index)->height();
        if( fabs(ray_height_at_stop - stop_radius) < eps){
            do_apply_vig_ = orig_vig_state;
            do_aperture_check_ = orig_aperture_check;
            return 0.0;
        }else{
            a = -1.0;
        }
    }


    int loop_cnt = 0;

    auto ray_m = std::make_shared<Ray>(path.size());

    while(fabs(a-b) > eps  && loop_cnt < max_loop_cnt){
        loop_cnt++;

        double m = (a+b)/2.0;
        vig_pupil(0) = full_pupil(0)*(1.0 - m);
        vig_pupil(1) = full_pupil(1)*(1.0 - m);

        trace_result = trace_pupil_ray(ray_m, path, vig_pupil, &fld, ref_wvl);

        if(TRACE_BLOCKED_ERROR == trace_result){
            a = m;
            continue;
        }else if(TRACE_SUCCESS == trace_result){
            b = m;
        }else{
            do_apply_vig_ = orig_vig_state;
            do_aperture_check_ = orig_aperture_check;
        }
    }

    vig = b;

    do_apply_vig_ = orig_vig_state;
    do_aperture_check_ = orig_aperture_check;

    return vig;
}


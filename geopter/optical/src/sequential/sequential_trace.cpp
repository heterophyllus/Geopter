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
#include "sequential/trace_error.h"
#include "paraxial/paraxial_trace.h"

using namespace geopter;

SequentialTrace::SequentialTrace(OpticalSystem* sys):
    opt_sys_(sys)
{
    fund_data_ = opt_sys_->fundamental_data();

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

    double eprad = fund_data_.enp_radius;
    Eigen::Vector2d aim_pt = fld->aim_pt();

    double obj_dist = fund_data_.object_distance;
    double enp_dist = fund_data_.enp_distance;

    pt0 = this->object_coord(fld);

    Eigen::Vector3d pt1;
    pt1(0) = eprad*vig_pupil(0) + aim_pt(0);
    pt1(1) = eprad*vig_pupil(1) + aim_pt(1);
    pt1(2) = obj_dist + enp_dist;

    dir0 = pt1 - pt0;
    dir0.normalize();
}

RayPtr SequentialTrace::trace_pupil_ray(const Eigen::Vector2d& pupil_crd, const Field *fld, double wvl)
{
    Eigen::Vector3d pt0;
    Eigen::Vector3d dir0;

    pupil_coord_to_obj(pt0, dir0, pupil_crd, fld);


    SequentialPath path = sequential_path(0, fund_data_.image_surface_index, wvl);

    std::shared_ptr<Ray> ray = trace_ray_throughout_path(path, pt0, dir0);
    ray->set_pupil_coord(pupil_crd);
    ray->set_wvl(wvl);
    return ray;

}

void SequentialTrace::trace_pupil_ray(RayPtr ray, const SequentialPath &seq_path, const Eigen::Vector2d &pupil_crd, const Field *fld, double wvl)
{
    Eigen::Vector3d pt0;
    Eigen::Vector3d dir0;

    ray->set_pupil_coord(pupil_crd);
    ray->set_wvl(wvl);

    pupil_coord_to_obj(pt0, dir0, pupil_crd, fld);
    trace_ray_throughout_path(ray, seq_path, pt0, dir0);
}

void SequentialTrace::trace_reference_rays(std::vector<RayPtr> &ref_rays, const Field *fld, double wvl)
{
    ref_rays.clear();

    constexpr int num_rays = 5;
    std::vector<Eigen::Vector2d> pupils(num_rays);
    pupils[0] = Eigen::Vector2d({0.0, 0.0});
    pupils[1] = Eigen::Vector2d({0.0, 1.0});
    pupils[2] = Eigen::Vector2d({0.0, -1.0});
    pupils[3] = Eigen::Vector2d({1.0, 0.0});
    pupils[4] = Eigen::Vector2d({-1.0, 0.0});

    for(int i = 0; i < num_rays; i++){
        std::shared_ptr<Ray> ray ;
        try{
            ray = trace_pupil_ray(pupils[i], fld, wvl);
        }catch(TraceError &e){
            ray = e.ray();
        }
        ref_rays.push_back(ray);
    }
}


RayPtr SequentialTrace::trace_ray_throughout_path(const SequentialPath& seq_path, const Eigen::Vector3d& pt0, const Eigen::Vector3d& dir0)
{
    std::shared_ptr<Ray> ray = std::make_shared<Ray>();

    if(seq_path.size() == 0) {
        return ray; // empty ray
    }

    Eigen::Vector3d before_pt  = pt0;
    Eigen::Vector3d before_dir = dir0;
    Eigen::Vector3d intersect_pt;
    Eigen::Vector3d after_dir;
    double distance_from_before = 0.0;
    double n_out = seq_path.at(0).rind;
    double n_in  = n_out;
    Transformation transformation_from_before = seq_path.at(0).srf->local_transform();
    double op_delta = 0.0;
    double opl = 0.0;

    constexpr double eps = 1.0e-7;
    constexpr double z_dir = 1.0; // used for reflection, not yet implemented


    // first surface
    Eigen::Vector3d srf_normal_1st = seq_path.at(0).srf->normal(pt0);
    ray->append(pt0, srf_normal_1st, dir0, 0.0, 0.0);

    int path_size = seq_path.size();

    if (path_size == 1){
        return ray;
    }

    // trace ray throughout the path till the image
    Eigen::Matrix3d rt;
    Eigen::Vector3d t, rel_before_pt, rel_before_dir, foot_of_perpendicular_pt, srf_normal;
    int cur_srf_idx = 1;
    try {
        for(cur_srf_idx = 1; cur_srf_idx < path_size; cur_srf_idx++) {

            rt = transformation_from_before.rotation;
            t  = transformation_from_before.transfer;

            rel_before_pt = rt*(before_pt - t); // relative source point looked from current surface
            rel_before_dir = rt*before_dir;     // relative ray direction looked from current surface

            double dist_from_before_to_perpendicular = -rel_before_pt.dot(rel_before_dir); // distance from previous point to foot of perpendicular
            foot_of_perpendicular_pt = rel_before_pt + dist_from_before_to_perpendicular*rel_before_dir; // foot of perpendicular from the current surface apex to the incident ray line

            Surface* cur_srf = seq_path.at(cur_srf_idx).srf;
            double dist_from_perpendicular_to_intersect_pt; // distance from the foot of perpendicular to the intersect point
            cur_srf->intersect(intersect_pt, dist_from_perpendicular_to_intersect_pt, foot_of_perpendicular_pt, rel_before_dir, eps, z_dir);

            distance_from_before = dist_from_before_to_perpendicular + dist_from_perpendicular_to_intersect_pt; // distance between before and current intersect point

            n_out = seq_path.at(cur_srf_idx).rind;
            srf_normal = cur_srf->normal(intersect_pt); // surface normal at the intersect point
            after_dir = bend(before_dir, srf_normal, n_in, n_out);

            opl = n_in * distance_from_before;

            ray->append(intersect_pt, srf_normal, after_dir.normalized(),distance_from_before, opl);

            if(do_aperture_check_) {
                if( !cur_srf->point_inside(intersect_pt(0),intersect_pt(1)) ){
                    throw TraceBlockedByApertureError();
                }
            }

            before_pt  = intersect_pt;
            before_dir = after_dir;
            n_in       = n_out;
            transformation_from_before = cur_srf->local_transform();

        }

    } catch (TraceMissedSurfaceError &e) {
        ray->set_status(RayStatus::MissedSurface);
        e.set_ray(ray);
        e.set_surface_index(cur_srf_idx);
        throw e;

    } catch (TraceTIRError& e){
        ray->set_status(RayStatus::TotalReflection);
        e.set_ray(ray);
        e.set_surface_index(cur_srf_idx);
        throw e;

    } catch (TraceBlockedByApertureError& e) {
        ray->set_status(RayStatus::Blocked);
        e.set_ray(ray);
        e.set_surface_index(cur_srf_idx);
        throw e;

    }

    op_delta += opl;

    return ray;
}

void SequentialTrace::trace_ray_throughout_path(RayPtr ray, const SequentialPath &seq_path, const Eigen::Vector3d &pt0, const Eigen::Vector3d &dir0)
{
    const int path_size = seq_path.size();

    if(ray->size() != path_size){
        ray->init(path_size);
    }

    Eigen::Vector3d before_pt  = pt0;
    Eigen::Vector3d before_dir = dir0;
    Eigen::Vector3d intersect_pt;
    Eigen::Vector3d after_dir;
    double distance_from_before = 0.0;
    double n_out = seq_path.at(0).rind;
    double n_in  = n_out;
    Transformation transformation_from_before = seq_path.at(0).srf->local_transform();
    double op_delta = 0.0;
    double opl = 0.0;

    constexpr double eps = 1.0e-7;
    constexpr double z_dir = 1.0; // used for reflection, not yet implemented


    // first surface
    Eigen::Vector3d srf_normal_1st = seq_path.at(0).srf->normal(pt0);
    ray->at(0)->set_data(pt0, srf_normal_1st, dir0, 0.0, 0.0);


    // trace ray throughout the path till the image
    Eigen::Matrix3d rt;
    Eigen::Vector3d t, rel_before_pt, rel_before_dir, foot_of_perpendicular_pt, srf_normal;
    int cur_srf_idx = 1;
    try {
        for(cur_srf_idx = 1; cur_srf_idx < path_size; cur_srf_idx++) {

            rt = transformation_from_before.rotation;
            t  = transformation_from_before.transfer;

            rel_before_pt = rt*(before_pt - t); // relative source point looked from current surface
            rel_before_dir = rt*before_dir;     // relative ray direction looked from current surface

            double dist_from_before_to_perpendicular = -rel_before_pt.dot(rel_before_dir); // distance from previous point to foot of perpendicular
            foot_of_perpendicular_pt = rel_before_pt + dist_from_before_to_perpendicular*rel_before_dir; // foot of perpendicular from the current surface apex to the incident ray line

            Surface* cur_srf = seq_path.at(cur_srf_idx).srf;
            double dist_from_perpendicular_to_intersect_pt; // distance from the foot of perpendicular to the intersect point
            cur_srf->profile()->intersect(intersect_pt, dist_from_perpendicular_to_intersect_pt, foot_of_perpendicular_pt, rel_before_dir, eps, z_dir);

            distance_from_before = dist_from_before_to_perpendicular + dist_from_perpendicular_to_intersect_pt; // distance between before and current intersect point

            n_out = seq_path.at(cur_srf_idx).rind;
            srf_normal = cur_srf->normal(intersect_pt); // surface normal at the intersect point
            after_dir = bend(before_dir, srf_normal, n_in, n_out);

            opl = n_in * distance_from_before;

            ray->at(cur_srf_idx)->set_data(intersect_pt, srf_normal, after_dir.normalized(),distance_from_before, opl);

            if(do_aperture_check_) {
                if( !cur_srf->point_inside(intersect_pt(0),intersect_pt(1)) ){
                    throw TraceBlockedByApertureError();
                }
            }

            before_pt  = intersect_pt;
            before_dir = after_dir;
            n_in       = n_out;
            transformation_from_before = cur_srf->local_transform();
        }

    } catch (TraceMissedSurfaceError &e) {
        ray->set_status(RayStatus::MissedSurface);
        e.set_ray(ray);
        e.set_surface_index(cur_srf_idx);
        return;
        //throw e;

    } catch (TraceTIRError& e){
        ray->set_status(RayStatus::TotalReflection);
        e.set_ray(ray);
        e.set_surface_index(cur_srf_idx);
        return;
        //throw e;

    } catch (TraceBlockedByApertureError& e) {
        ray->set_status(RayStatus::Blocked);
        e.set_ray(ray);
        e.set_surface_index(cur_srf_idx);
        return;
        //throw e;
    } catch(TraceError& e){
        ray->set_status(4);
        return;
    }

    //ray->set_status(RayStatus::PassThrough);
    //op_delta += opl;
}


GridArray<RayPtr> SequentialTrace::trace_grid_rays(const Field* fld, double wvl, int nrd)
{
    GridArray<RayPtr> ray_grid(nrd, nrd);

    SequentialPath path = sequential_path(wvl);

    Eigen::Vector2d pupil;
    Eigen::Vector3d pt0, dir0;
    RayPtr ray = nullptr;

    for(int i = 0; i < nrd; i++)
    {
        double py = -1.0 + (double)i*2.0/(double)(nrd-1);

        for(int j = 0; j < nrd; j++)
        {
            double px = -1.0 + (double)j*2.0/(double)(nrd-1);
            pupil(0) = px;
            pupil(1) = py;

            ray = nullptr;

            double r2 = pow(pupil(0),2) + pow(pupil(1),2);
            if(r2 < 1.0){
                pupil_coord_to_obj(pt0, dir0, pupil, fld);

                try{
                    ray = trace_ray_throughout_path(path, pt0, dir0);
                    ray->set_wvl(wvl);
                    ray->set_pupil_coord(pupil);

                }catch(TraceError &e){
                    ray = nullptr;
                }

            }

            ray_grid.set_cell(i, j, ray);
        }

    }

    return ray_grid;
}



HexapolarArray<RayPtr> SequentialTrace::trace_hexapolar_rays(const Field *fld, double wvl, int nrd)
{
    HexapolarArray<RayPtr> rays_hexapolar(nrd);

    SequentialPath path = sequential_path(wvl);

    Eigen::Vector2d pupil;
    Eigen::Vector3d pt0, dir0;
    RayPtr ray = nullptr;

    int half_num_rings = nrd/2;
    for (int r = 0; r < nrd/2; r++)
    {
        int num_rays_in_ring = 6*r;

        // center of hexapolar
        if(num_rays_in_ring == 0){
            pupil(0) = 0.0;
            pupil(1) = 0.0;
            pupil_coord_to_obj(pt0, dir0, pupil, fld);
            try{
                ray = trace_ray_throughout_path(path, pt0, dir0);
                ray->set_wvl(wvl);
                ray->set_pupil_coord(pupil);
            }catch(TraceError &e){
                ray = nullptr;
            }

            rays_hexapolar[0] = ray;

            continue;
        }

        double ang_step = 2*M_PI/(double)num_rays_in_ring;

        for(int ai = 0; ai < num_rays_in_ring; ai++){
            pupil(0) = (double)r * 1.0/(half_num_rings) * cos((double)ai*ang_step);
            pupil(1) = (double)r * 1.0/(half_num_rings) * sin((double)ai*ang_step);

            pupil_coord_to_obj(pt0, dir0, pupil, fld);
            try{
                ray = trace_ray_throughout_path(path, pt0, dir0);
                ray->set_wvl(wvl);
                ray->set_pupil_coord(pupil);
            }catch(TraceError &e){
                ray = nullptr;
            }

            rays_hexapolar[ HexapolarArray<Ray>::ring_azimuth_to_index(r, ai) ] = ray;
        }
    }

    return rays_hexapolar;

}


Eigen::Vector2d SequentialTrace::trace_coddington(const Field *fld, double wvl, double offset)
{
    /* R. Kingslake, "Lens Design Fundamentals", p292 */

    Eigen::Vector2d s_t;

    std::shared_ptr<Ray> ray;

    try{
        ray = trace_pupil_ray(Eigen::Vector2d({0.0, offset}), fld, wvl);
    }catch(TraceError &e){
        throw e;
    }

    SequentialPath path = sequential_path(wvl);

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
        //double B = opt_sys_->optical_assembly()->gap(0)->thi();
        double B = fund_data_.object_distance;
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
        if(surf->profile()->name() == "SPH") {
            double c = surf->profile()->cv();
            obl_pwr_s = c*(n_after*cosI_prime - n_before*cosI);
            obl_pwr_t = obl_pwr_s;

        }else{ // aspherical

            double y = ray->at(i)->y();

            if(fabs(y) < std::numeric_limits<double>::epsilon()){
                double cs = surf->profile()->cv();
                obl_pwr_s = cs*(n_after*cosI_prime - n_before*cosI);
            }else{
                double sinI_U = sinI*cosU - cosI*sinU;
                double cs = sinI_U/y;
                obl_pwr_s = cs*(n_after*cosI_prime - n_before*cosI);
            }

            double d2z_dy2 = surf->profile()->deriv_2nd(y);
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
    return sequential_path(0, fund_data_.image_surface_index, wvl);
}

SequentialPath SequentialTrace::sequential_path(int start, int end, double wvl)
{
    assert(end <= fund_data_.image_surface_index);

    SequentialPath path;
    SequentialPathComponent path_comp;

    const int num_srf = fund_data_.number_of_surfaces;
    const int num_gap = fund_data_.number_of_surfaces -1;

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


Eigen::Vector2d SequentialTrace::aim_chief_ray(const Field *fld, double wvl)
{
    int stop = opt_sys_->optical_assembly()->stop_index();
    Eigen::Vector2d xy_target({0.0, 0.0});

    try{
        Eigen::Vector2d aim_pt = search_aim_point(stop, xy_target, fld, wvl);

        return aim_pt;
    }catch(TraceRayAimingFailedError &e){
        throw e;
    }
}


Eigen::Vector2d SequentialTrace::search_aim_point(int srf_idx, const Eigen::Vector2d& xy_target, const Field *fld, double wvl)
{
    assert(srf_idx > 0);

    double enp_dist = opt_sys_->paraxial_data()->entrance_pupil_distance();
    double obj_dist = opt_sys_->optical_assembly()->gap(0)->thi();
    double obj2enp_dist = obj_dist + enp_dist;

    Eigen::Vector3d pt0 = this->object_coord(fld);

    Eigen::Vector2d start_coords;
    double start_y;
    double y_target = xy_target(1);


    // newton
    int cnt = 0;
    constexpr int max_loop_cnt = 30;
    double x1,x2,y1,y2;
    double diff_x;
    double next_x;
    constexpr double delta = 1.0e-5;
    constexpr double error = 1.0e-5;

    x1 = 0.0;

    while(true) {
        cnt++;
        x2 = x1 + delta;

        if(cnt > max_loop_cnt){
            x1 = 0.0;
            break;
        }

        y1 = y_stop_coordinate(x1,srf_idx,pt0, obj2enp_dist, wvl, y_target);
        y2 = y_stop_coordinate(x2,srf_idx,pt0, obj2enp_dist, wvl, y_target);

        if(std::isnan(y1+y2)){
            continue;
        }
        else if(fabs(y1 - y_target) < error) {
            break;
        }

        diff_x = (y2 - y1) / (x2 - x1);
        next_x = x1 - y1/diff_x;

        x1 = next_x;
    }

    start_y = x1;

    start_coords(0) = 0.0;
    start_coords(1) = start_y;

    if(std::isnan(start_y)){
        throw TraceRayAimingFailedError();
    }

    return start_coords;

}

double SequentialTrace::y_stop_coordinate(double y1, int ifcx, const Eigen::Vector3d& pt0, double dist, double wvl, double y_target)
{
    Eigen::Vector3d pt1({0.0, y1, dist});
    Eigen::Vector3d dir0 = pt1 - pt0;
    dir0.normalize();

    std::shared_ptr<Ray> ray;

    try{
        ray = trace_ray_throughout_path(sequential_path(wvl), pt0, dir0);
        double y_ray = ray->at(ifcx)->y();
        return (y_ray - y_target);

    }catch(TraceError &e){
        ray = e.ray();
        if(ifcx < ray->size()){
            double y_ray = ray->at(ifcx)->y();

            return (y_ray - y_target);
        }else{
            throw e;
        }
    }

}

Eigen::Vector3d SequentialTrace::bend(const Eigen::Vector3d& d_in, const Eigen::Vector3d& normal, double n_in, double n_out)
{
    double normal_len = normal.norm();
    double cosI = d_in.dot(normal)/normal_len;
    double sinI_sqr = 1.0 - cosI*cosI;

    double inside_sqrt = n_out*n_out - n_in*n_in*sinI_sqr;
    if(inside_sqrt < 0.0){
        throw TraceTIRError();
    }

    double cosI_sgn = (cosI > 0.0) - (cosI < 0.0);
    //double n_cosIp = sqrt(n_out*n_out - n_in*n_in*sinI_sqr) * cosI_sgn;
    double n_cosIp = sqrt( inside_sqrt ) * cosI_sgn;
    double alpha = n_cosIp - n_in*cosI;
    Eigen::Vector3d d_out = (n_in*d_in + alpha*normal)/n_out;

    return d_out;
}

Eigen::Vector3d SequentialTrace::object_coord(const Field* fld)
{
    Eigen::Vector3d obj_pt;
    Eigen::Vector3d ang_dg;
    Eigen::Vector3d img_pt;
    Eigen::Vector3d dir_tan;

    auto parax_data = opt_sys_->paraxial_data();

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
        obj_pt = -dir_tan*(parax_data->object_distance() + parax_data->entrance_pupil_distance());
        break;

    case FieldType::OBJ_HT:
        obj_pt(0) = fld_x;
        obj_pt(1) = fld_y;
        obj_pt(2) = 0.0;
        break;

    case FieldType::IMG_HT:
        img_pt = Eigen::Vector3d({fld_x, fld_y, 0.0});
        obj_pt = parax_data->reduction_rate()*img_pt;
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

    std::vector<double> vig_factors = std::vector<double>( {vuy, vly, vux, vlx});

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
    bool orig_vig_state = do_apply_vig_;
    do_apply_vig_ = false;

    bool orig_aperture_check = do_aperture_check_;
    do_aperture_check_ = true;

    constexpr double eps = 1.0e-5;
    constexpr int max_loop_cnt = 30;

    const int stop_index = opt_sys_->optical_assembly()->stop_index();
    const double stop_radius = opt_sys_->optical_assembly()->surface(stop_index)->max_aperture();

    Eigen::Vector2d vig_pupil = full_pupil;

    double vig = 0.0;

    double a = 0.0;
    double b = 1.0;

    vig_pupil(0) = full_pupil(0)*(1.0 - a);
    vig_pupil(1) = full_pupil(1)*(1.0 - a);
    std::shared_ptr<Ray> ray_full_marginal;
    try{
        ray_full_marginal= trace_pupil_ray(vig_pupil, &fld, fund_data_.reference_wvl_value);

    }catch(TraceBlockedByApertureError &e){
        ray_full_marginal = e.ray();

    }catch(TraceError &e){ //TIR, MissedSurface
        if(e.ray()->size() <= stop_index){
            throw e;
        }else{
            ray_full_marginal = e.ray();
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

    while(fabs(a-b) > eps  && loop_cnt < max_loop_cnt){
        loop_cnt++;

        double m = (a+b)/2.0;
        vig_pupil(0) = full_pupil(0)*(1.0 - m);
        vig_pupil(1) = full_pupil(1)*(1.0 - m);

        std::shared_ptr<Ray> ray_m;
        try{
            ray_m = trace_pupil_ray(vig_pupil, &fld, fund_data_.reference_wvl_value);
        }catch(TraceBlockedByApertureError &e){
            a = m;
            continue;
        }catch(TraceError &e){
            do_apply_vig_ = orig_vig_state;
            do_aperture_check_ = orig_aperture_check;
            throw e;
        }

        b = m;
    }

    vig = b;

    do_apply_vig_ = orig_vig_state;
    do_aperture_check_ = orig_aperture_check;

    return vig;
}


void SequentialTrace::set_aperture_check(bool state)
{
    do_aperture_check_ = state;
}

bool SequentialTrace::aperture_check_state() const
{
    return do_aperture_check_;
}

void SequentialTrace::set_apply_vig(bool state)
{
    do_apply_vig_ = state;
}

bool SequentialTrace::apply_vig_status() const
{
    return do_apply_vig_;;
}

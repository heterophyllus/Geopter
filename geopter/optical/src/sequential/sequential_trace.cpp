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

#define _USE_MATH_DEFINES
#include <cmath>

#include "sequential/sequential_trace.h"

#include <limits>
#include <iostream>

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

void SequentialTrace::ConvertCoordinatePupilToObj(Eigen::Vector3d& pt0, Eigen::Vector3d& dir0, const Eigen::Vector2d& pupil_crd, const Field* fld)
{
    Eigen::Vector2d vig_pupil = pupil_crd;

    if(do_apply_vig_){
        vig_pupil = fld->ApplyVignetting(pupil_crd);
    }

    double eprad = opt_sys_->GetFirstOrderData()->entrance_pupil_radius;
    Eigen::Vector2d aim_pt = fld->AimPt();

    double obj_dist = opt_sys_->GetOpticalAssembly()->GetGap(0)->Thickness();
    double enp_dist = opt_sys_->GetFirstOrderData()->entrance_pupil_distance;

    pt0 = this->GetDefaultObjectPt(fld);

    Eigen::Vector3d pt1;
    pt1(0) = eprad*vig_pupil(0) + aim_pt(0);
    pt1(1) = eprad*vig_pupil(1) + aim_pt(1);
    pt1(2) = obj_dist + enp_dist;

    dir0 = pt1 - pt0;
    dir0.normalize();
}


TraceError SequentialTrace::TracePupilRay(RayPtr ray, const SequentialPath &seq_path, const Eigen::Vector2d &pupil_crd, const Field *fld, double wvl)
{
    Eigen::Vector3d pt0;
    Eigen::Vector3d dir0;

    ray->SetPupilCoordinate(pupil_crd);
    ray->SetWavelength(wvl);

    ConvertCoordinatePupilToObj(pt0, dir0, pupil_crd, fld);
    return TraceRayThroughoutPath(ray, seq_path, pt0, dir0);
}

RayPtr SequentialTrace::CreatePupilRay(const Eigen::Vector2d &pupil_crd, const Field *fld, double wvl)
{
    SequentialPath seq_path = CreateSequentialPath(wvl);
    auto ray = std::make_shared<Ray>(seq_path.Size());
    TracePupilRay(ray, seq_path, pupil_crd, fld, wvl);

    return ray;
}

bool SequentialTrace::TraceReferenceRays(std::vector<RayPtr> &ref_rays, const Field *fld, double wvl)
{
    const int num_srfs = opt_sys_->GetOpticalAssembly()->NumberOfSurfaces();

    SequentialPath seq_path = CreateSequentialPath(wvl);

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
        if( TRACE_SUCCESS != TracePupilRay(ray, seq_path, pupils[i], fld, wvl)){
            std::cerr << "Reference ray trace error" << std::endl;
            return false;
        }
        ref_rays.push_back(ray);
    }
    return true;
}



TraceError SequentialTrace::TraceRayThroughoutPath(RayPtr ray, const SequentialPath &seq_path, const Eigen::Vector3d &pt0, const Eigen::Vector3d &dir0)
{
    const int path_size = seq_path.Size();

    if(ray->NumberOfSegments() != path_size){
        ray->Allocate(path_size);
    }

    Eigen::Vector3d before_pt  = pt0;
    Eigen::Vector3d before_dir = dir0;
    Eigen::Vector3d intersect_pt;
    Eigen::Vector3d after_dir;
    double distance_from_before = 0.0;
    double n_out = seq_path.At(0).refractive_index;
    double n_in  = n_out;
    Transformation transformation_from_before = seq_path.At(0).surface->LocalTransform();
    //double op_delta = 0.0;
    double opl = 0.0;

    //constexpr double z_dir = 1.0; // used for reflection, not yet implemented


    // first surface
    Eigen::Vector3d srf_normal_1st = seq_path.At(0).surface->Normal(pt0);
    ray->GetSegmentAt(0)->SetData(pt0, srf_normal_1st, dir0, 0.0, 0.0);


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

        Surface* cur_srf = seq_path.At(cur_srf_idx).surface;
        double dist_from_perpendicular_to_intersect_pt; // distance from the foot of perpendicular to the intersect point

        if( ! cur_srf->Intersect(intersect_pt, dist_from_perpendicular_to_intersect_pt, foot_of_perpendicular_pt, rel_before_dir) ){
            ray->SetStatus(TRACE_MISSEDSURFACE_ERROR);
            ray->SetReachedSurfaceIndex(cur_srf_idx - 1);
            ray->GetSegmentAt(cur_srf_idx)->SetStatus(TRACE_MISSEDSURFACE_ERROR);
            return TRACE_MISSEDSURFACE_ERROR;
        }

        distance_from_before = dist_from_before_to_perpendicular + dist_from_perpendicular_to_intersect_pt; // distance between before and current intersect point

        n_out = seq_path.At(cur_srf_idx).refractive_index;
        srf_normal = cur_srf->Normal(intersect_pt); // surface normal at the intersect point
        if( ! Bend(after_dir ,before_dir, srf_normal, n_in, n_out) ){
            ray->SetStatus(TRACE_TIR_ERROR);
            ray->SetReachedSurfaceIndex(cur_srf_idx);
            ray->GetSegmentAt(cur_srf_idx)->SetData(intersect_pt, srf_normal, after_dir.normalized(),distance_from_before, opl);
            ray->GetSegmentAt(cur_srf_idx)->SetStatus(TRACE_TIR_ERROR);
            return TRACE_TIR_ERROR;
        }

        opl = n_in * distance_from_before;

        ray->GetSegmentAt(cur_srf_idx)->SetData(intersect_pt, srf_normal, after_dir.normalized(),distance_from_before, opl);
        ray->GetSegmentAt(cur_srf_idx)->SetStatus(TRACE_SUCCESS);

        if(do_aperture_check_) {
            if( !cur_srf->PointInside(intersect_pt(0),intersect_pt(1)) ){
                ray->SetStatus(TRACE_BLOCKED_ERROR);
                ray->SetReachedSurfaceIndex(cur_srf_idx);
                ray->GetSegmentAt(cur_srf_idx)->SetStatus(TRACE_BLOCKED_ERROR);
                return TRACE_BLOCKED_ERROR;
            }
        }

        before_pt  = intersect_pt;
        before_dir = after_dir;
        n_in       = n_out;
        transformation_from_before = cur_srf->LocalTransform();
    }

    //op_delta += opl;

    ray->SetStatus(TRACE_SUCCESS);
    ray->SetReachedSurfaceIndex(path_size-1);
    ray->GetSegmentAt(path_size-1)->SetStatus(TRACE_SUCCESS);

    return TRACE_SUCCESS;
}



bool SequentialTrace::TraceCoddington(Eigen::Vector2d &s_t, const std::shared_ptr<Ray> ray, const SequentialPath& path)
{
    /* R. Kingslake, "Lens Design Fundamentals", p292 */

    if( ray->Status() != TRACE_SUCCESS ){
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
    if(std::isinf(path.At(0).distance)){
        s_before = std::numeric_limits<double>::infinity();
        t_before = std::numeric_limits<double>::infinity();
    }else{
        Eigen::Vector3d dir0 = ray->GetSegmentAt(0)->Direction();
        double cosUpr = dir0(2);
        double B = -path.At(0).distance; // object distance
        double Zpr = ray->GetSegmentAt(1)->Z();

        s_before = (B - Zpr)/cosUpr;
        t_before = s_before;
    }

    n_before = path.At(0).refractive_index;

    int num_srf = ray->NumberOfSegments();
    for(int i = 1; i < num_srf-1; i++) {
        n_after = path.At(i).refractive_index;
        double cosI = cos(ray->GetSegmentAt(i)->AngleOfIncidence());
        double cosI_prime = cos(ray->GetSegmentAt(i)->AngleOfRefraction());
        double sinI = sin(ray->GetSegmentAt(i)->AngleOfIncidence());

        cosU = ray->GetSegmentAt(i-1)->N();
        cosU_prime = ray->GetSegmentAt(i)->N();
        double sinU = sqrt(1.0 - cosU*cosU);

        Surface* surf = path.At(i).surface;
        if (surf->IsProfile<Spherical>()) {
            double c = surf->Curvature();
            obl_pwr_s = c*(n_after*cosI_prime - n_before*cosI);
            obl_pwr_t = obl_pwr_s;

        }else{ // aspherical

            double y = ray->GetSegmentAt(i)->Y();

            if(fabs(y) < std::numeric_limits<double>::epsilon()){
                double cs = surf->Curvature();
                obl_pwr_s = cs*(n_after*cosI_prime - n_before*cosI);
            }else{
                double sinI_U = sinI*cosU - cosI*sinU;
                double cs = sinI_U/y;
                obl_pwr_s = cs*(n_after*cosI_prime - n_before*cosI);
            }

            double d2z_dy2 = 0.0;
            if(surf->IsProfile<EvenPolynomial>()){
                d2z_dy2 = surf->Profile<EvenPolynomial>()->deriv_2nd(y);
            }else{
                d2z_dy2 = surf->Profile<OddPolynomial>()->deriv_2nd(y);
            }

            double cosI_U = cosI*cosU + sinI*sinU;
            double ct = d2z_dy2 * pow( cosI_U, 3);
            obl_pwr_t = ct*(n_after*cosI_prime - n_before*cosI);
        }


        double z1 = ray->GetSegmentAt(i)->Z();
        double z2 = ray->GetSegmentAt(i+1)->Z();
        double d = path.At(i).distance;

        double D = (d + z2 - z1)/cosU_prime;

        s_after = n_after/(n_before/s_before + obl_pwr_s);
        t_after = n_after*cosI_prime*cosI_prime / ( (n_before*cosI*cosI/t_before) + obl_pwr_t );

        s_before = s_after - D;
        t_before = t_after - D;

        n_before = n_after;

    }


    // Closing Equation
    double img_dist = opt_sys_->GetOpticalAssembly()->ImageSpaceGap()->Thickness();

    double z = ray->GetSegmentAt(ray->NumberOfSegments()-1-1)->Z();

    double zs = s_after*cosU_prime + z - img_dist;
    double zt = t_after*cosU_prime + z - img_dist;

    s_t(0) = zs;
    s_t(1) = zt;

    return true;
}

SequentialPath SequentialTrace::CreateSequentialPath(double wvl)
{
    const int img = opt_sys_->GetOpticalAssembly()->ImageIndex();
    return CreateSequentialPath(0, img, wvl);
}

SequentialPath SequentialTrace::CreateSequentialPath(int start, int end, double wvl)
{
    const int img = opt_sys_->GetOpticalAssembly()->ImageIndex();

    assert(end <= img);

    SequentialPath path;
    SequentialPathComponent path_comp;

    const int num_srf = opt_sys_->GetOpticalAssembly()->NumberOfSurfaces();
    const int num_gap = num_srf -1;

    for(int i = start; i <= end; i++)
    {
        if ( i < num_srf ) {
            path_comp.surface = opt_sys_->GetOpticalAssembly()->GetSurface(i);
        }else{
            path_comp.surface = nullptr;
        }

        if( i < num_gap ) {
            path_comp.distance         = opt_sys_->GetOpticalAssembly()->GetGap(i)->Thickness();
            path_comp.refractive_index = opt_sys_->GetOpticalAssembly()->GetGap(i)->GetMaterial()->RefractiveIndex(wvl);
        }else {
            path_comp.distance         = 0.0;
            path_comp.refractive_index = 1.0;
        }

        path.Append(path_comp);
    }


    return path;
}


bool SequentialTrace::AimChiefRay(Eigen::Vector2d& aim_pt, Eigen::Vector3d& obj_pt, const Field *fld, double wvl)
{
    int stop = opt_sys_->GetOpticalAssembly()->StopIndex();
    Eigen::Vector2d xy_target({0.0, 0.0});

    SequentialPath seq_path = CreateSequentialPath(wvl);

    auto ray = std::make_shared<Ray>(seq_path.Size());
    bool result = SearchRayAimingAtSurface(ray, aim_pt, fld, stop, xy_target);

    if(result){
        obj_pt = ray->GetSegmentAt(0)->IntersectPt();
        return true;
    }else{
        return false;
    }

    //return search_aim_point(aim_pt, stop, xy_target, fld, wvl);
}



bool SequentialTrace::SearchRayAimingAtSurface(RayPtr ray, Eigen::Vector2d& aim_pt, const Field *fld, int target_srf_idx, const Eigen::Vector2d &xy_target)
{
    //const int fld_type = opt_sys_->optical_spec()->field_of_view()->field_type();
    double ref_wvl = opt_sys_->GetOpticalSpec()->GetWavelengthSpec()->ReferenceWavelength();

    double obj_dist = opt_sys_->GetOpticalAssembly()->GetGap(0)->Thickness();
    double enp_dist = opt_sys_->GetFirstOrderData()->entrance_pupil_distance;

    SequentialPath seq_path = CreateSequentialPath(ref_wvl);

    Eigen::Vector3d pt0 = this->GetDefaultObjectPt(fld);
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

    ray->Allocate(seq_path.Size());

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

        ray->SetPupilCoordinate(aim_pt);
        trace_result1 = TraceRayThroughoutPath(ray, seq_path, pt0, dir0);
        if(trace_result1 == TRACE_SUCCESS){
            y_ray1 = ray->GetSegmentAt(target_srf_idx)->Y();
        }else{
            if(ray->GetReachedSurfaceIndex() >= target_srf_idx){
                y_ray1 = ray->GetSegmentAt(target_srf_idx)->Y();
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

        ray->SetPupilCoordinate(aim_pt);
        trace_result2 = TraceRayThroughoutPath(ray, seq_path, pt0, dir0);
        if(trace_result2 == TRACE_SUCCESS){
            y_ray2 = ray->GetSegmentAt(target_srf_idx)->Y();
        }else{
            if(ray->GetReachedSurfaceIndex() >= target_srf_idx){
                y_ray2 = ray->GetSegmentAt(target_srf_idx)->Y();
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

bool SequentialTrace::Bend(Eigen::Vector3d& d_out, const Eigen::Vector3d& d_in, const Eigen::Vector3d& normal, double n_in, double n_out)
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

Eigen::Vector3d SequentialTrace::GetDefaultObjectPt(const Field* fld)
{
    Eigen::Vector3d obj_pt;
    Eigen::Vector3d ang_dg;
    Eigen::Vector3d img_pt;
    Eigen::Vector3d dir_tan;

    auto fod = opt_sys_->GetFirstOrderData();

    int field_type = opt_sys_->GetOpticalSpec()->GetFieldSpec()->FieldType();
    double fld_x = fld->X();
    double fld_y = fld->Y();

    switch (field_type)
    {
    case FieldType::OBJ_ANG:
        ang_dg = Eigen::Vector3d({fld_x, fld_y, 0.0});
        dir_tan(0) = tan(ang_dg(0) * M_PI/180.0);
        dir_tan(1) = tan(ang_dg(1) * M_PI/180.0);
        dir_tan(2) = tan(ang_dg(2) * M_PI/180.0);
        obj_pt = -dir_tan*(fod->object_distance + fod->entrance_pupil_distance);
        break;

    case FieldType::OBJ_HT:
        obj_pt(0) = fld_x;
        obj_pt(1) = fld_y;
        obj_pt(2) = 0.0;
        break;

    case FieldType::IMG_HT:
        img_pt = Eigen::Vector3d({fld_x, fld_y, 0.0});
        obj_pt = fod->reduction*img_pt;
        break;

    default:
        obj_pt = Eigen::Vector3d::Zero(3);
    }

    return obj_pt;
}


std::vector<double> SequentialTrace::ComputeVignettingFactors(const Field& fld)
{
    double vuy = fld.VUY();
    double vly = fld.VLY();
    double vux = fld.VUX();
    double vlx = fld.VLX();

    std::vector<double> vig_factors = std::vector<double>( {vuy, vly, vux, vlx} );

    // check whether optical system has apertures
    const int img = opt_sys_->GetOpticalAssembly()->NumberOfSurfaces() - 1;
    bool has_aperture = false;
    for(int i = 0; i < img-1; i++){
        if( ! opt_sys_->GetOpticalAssembly()->GetSurface(i)->IsAperture<NoneAperture>()){
            has_aperture = true;
            break;
        }
    }

    if( ! has_aperture ){
        for(auto &v : vig_factors){
            v = 0.0;
        }
        return vig_factors;
    }


    std::vector<Eigen::Vector2d> pupils({Eigen::Vector2d({0.0, 1.0}),
                                         Eigen::Vector2d({0.0, -1.0}),
                                         Eigen::Vector2d({1.0, 0.0}),
                                         Eigen::Vector2d({-1.0, 0.0})});

    for(int i = 0; i < (int)pupils.size(); i++)
    {
        double vig = ComputeVignettingFactorForPupil(pupils[i], fld);
        vig_factors[i] = vig;
    }

    return vig_factors;
}


double SequentialTrace::ComputeVignettingFactorForPupil(const Eigen::Vector2d& full_pupil, const Field &fld)
{
    // save current setting
    bool orig_vig_state = do_apply_vig_;
    do_apply_vig_ = false;

    bool orig_aperture_check = do_aperture_check_;
    do_aperture_check_ = true;

    constexpr double eps = 1.0e-5;
    constexpr int max_loop_cnt = 30;

    const int stop_index = opt_sys_->GetOpticalAssembly()->StopIndex();
    const double stop_radius = opt_sys_->GetOpticalAssembly()->GetSurface(stop_index)->MaxAperture();

    const double ref_wvl = opt_sys_->GetOpticalSpec()->GetWavelengthSpec()->ReferenceWavelength();
    SequentialPath path = CreateSequentialPath(ref_wvl);

    Eigen::Vector2d vig_pupil = full_pupil;

    double vig = 0.0;

    double a = 0.0;
    double b = 1.0;

    vig_pupil(0) = full_pupil(0)*(1.0 - a);
    vig_pupil(1) = full_pupil(1)*(1.0 - a);
    std::shared_ptr<Ray> ray_full_marginal = std::make_shared<Ray>(path.Size());

    TraceError trace_result = TracePupilRay(ray_full_marginal, path, vig_pupil, &fld, ref_wvl);

    if(TRACE_SUCCESS != trace_result){
        if(ray_full_marginal->GetReachedSurfaceIndex() <= stop_index){
            return NAN;
        }
    }


    if(ray_full_marginal->Status() == TRACE_SUCCESS){
        double ray_height_at_stop = ray_full_marginal->GetSegmentAt(stop_index)->Height();
        if( fabs(ray_height_at_stop - stop_radius) < eps){
            do_apply_vig_ = orig_vig_state;
            do_aperture_check_ = orig_aperture_check;
            return 0.0;
        }else{
            a = -1.0;
        }
    }


    int loop_cnt = 0;

    auto ray_m = std::make_shared<Ray>(path.Size());

    while(fabs(a-b) > eps  && loop_cnt < max_loop_cnt){
        loop_cnt++;

        double m = (a+b)/2.0;
        vig_pupil(0) = full_pupil(0)*(1.0 - m);
        vig_pupil(1) = full_pupil(1)*(1.0 - m);

        trace_result = TracePupilRay(ray_m, path, vig_pupil, &fld, ref_wvl);

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


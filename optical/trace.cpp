#define _USE_MATH_DEFINES
#include <math.h>

#include <iomanip>
#include <fstream>
#include <limits>

#include "trace.h"
#include "trace_error.h"

#include "optical_model.h"
#include "sequential_model.h"
#include "paraxial_model.h"
#include "paraxial_ray.h"
#include "optical_spec.h"
#include "pupil_spec.h"
#include "field_spec.h"
#include "wvl_spec.h"
#include "surface.h"
#include "gap.h"
#include "aperture.h"
#include "surface_profile.h"

using namespace geopter;

Trace::Trace()
{

}

Ray Trace::trace_pupil_ray(const OpticalModel& opt_model, PupilCrd pupil, const Field& fld, double wvl)
{
    auto vig_pupil = fld.apply_vignetting(pupil);
    auto osp = opt_model.optical_spec();
    //auto fod = osp->paraxial_data().fod;
    //opt_model.paraxial_model()->compute_first_order();
    auto fod = opt_model.paraxial_model()->first_order_data();
    double eprad = fod.enp_radius;
    PupilCrd aim_pt;
    //aim_pt(0) = 0.0;
    //aim_pt(1) = 0.0;
    aim_pt = fld.aim_pt();

    Eigen::Vector3d pt0 = osp->obj_coord(fld);

    Eigen::Vector3d pt1;
    pt1(0) = eprad*vig_pupil(0) + aim_pt(0);
    pt1(1) = eprad*vig_pupil(1) + aim_pt(1);
    pt1(2) = fod.obj_dist + fod.enp_dist;

    Eigen::Vector3d dir0;
    if(std::isinf(fod.obj_dist)){

    }else{
        dir0 = pt1 - pt0;
    }
    double length = dir0.norm();

    //dir0 = dir0/length;
    dir0.normalize();

    return Trace::trace_ray_from_object(*opt_model.seq_model(), pt0, dir0, wvl);

}

Ray Trace::trace_pupil_ray(const OpticalModel& opt_model, Eigen::Vector2d pupil, int fi, int wi)
{
    auto fld = opt_model.optical_spec()->field_of_view()->field(fi);
    double wvl = opt_model.optical_spec()->spectral_region()->wvl(wi)->value();
    return trace_pupil_ray(opt_model, pupil, *fld, wvl);
}


std::vector<Ray> Trace::trace_fan_rays(const OpticalModel &opt_model, const Field &fld, double wvl, double azimuth, int nrd)
{
    const double step = 2.0/(double)(nrd-1);

    PupilCrd pupil;
    PupilCrd vig_pupil;

    Ray ray;
    std::vector<Ray> rays;

    auto deg2rad = [](double deg) { return deg*M_PI/180.0; };

    for(int ri = 0; ri < nrd; ri++){
        pupil(0) = (-1.0 + (double)ri*step) * cos(deg2rad(azimuth));
        pupil(1) = (-1.0 + (double)ri*step) * sin(deg2rad(azimuth));

        ray = trace_pupil_ray(opt_model, pupil, fld, wvl);
        rays.push_back(ray);
    }

    return rays;
}


Eigen::Vector2d Trace::trace_coddington(const OpticalModel &opt_model, const Field &fld, double wvl)
{
    /*
     * R. Kingslake, "Lens Design Fundamentals", p292
     */

    Eigen::Vector2d s_t;

    Path path = opt_model.seq_model()->path(wvl);

    // on axis
    double l = -path[0].gap->thi();
    double l_prime;
    double n_before = path[0].rndx;
    double n_after;
    for(int i = 1; i < (int)path.size()-1; i++) {

        n_after = path[i].rndx;

        double c = path[i].srf->profile()->cv();
        double d = path[i].gap->thi();
        double phi = c*(n_after - n_before);
        l_prime = n_after/(phi + (n_before/l));

        n_before = n_after;
        l = l_prime - d;
    }

    // Here, we have l_prime

    // off axis
    Ray ray = trace_pupil_ray(opt_model, PupilCrd({0.0, 0.0}), fld, wvl);

    double s_before, t_before, s_after, t_after;
    double obl_pwr_s, obl_pwr_t;
    double sinI, sinI_prime, cosI, cosI_prime, sinU, sinU_prime, cosU, cosU_prime;


    // Opening Equation
    if(std::isinf(path[0].gap->thi())){
        s_before = std::numeric_limits<double>::infinity();
        t_before = std::numeric_limits<double>::infinity();
    }else{
        auto dir0 = ray.at(0).after_dir;
        double cosUpr = dir0(2);
        double B = opt_model.seq_model()->gap(0)->thi();
        double Zpr = ray.z(1);

        s_before = (B - Zpr)/cosUpr;
        t_before = s_before;
    }


    for(int i = 1; i < ray.size()-1; i++) {
        Surface* surf = path[i].srf;

        n_before = path[i-1].rndx;
        n_after = path[i].rndx;
        cosI = cos(ray.aoi(i));
        cosI_prime = cos(ray.aor(i));
        //sinI = sqrt(1.0 - cosI*cosI);
        sinI = sin(ray.aoi(i));
        sinI_prime = sinI * n_before/n_after;
        //cosI_prime = sqrt(1.0 - sinI_prime*sinI_prime);

        cosU = ray.at(i-1).after_dir(2);//ray.at(i-1).after_dir.norm();
        sinU = sqrt(1.0 - cosU*cosU);
        cosU_prime = ray.at(i).after_dir(2);//ray.at(i).after_dir.norm();

        if(surf->profile()->name() == "SPH") {
            double c = surf->profile()->cv();
            obl_pwr_s = c*(n_after*cosI_prime - n_before*cosI);
            obl_pwr_t = obl_pwr_s;
        }else{ // aspherical
            double sinI_U = sinI*cosU - cosI*sinU;
            double y = ray.y(i);
            double cs = sinI_U/y;
            obl_pwr_s = cs*(n_after*cosI_prime - n_before*cosI);

            double d2z_dy2 = surf->profile()->deriv_2nd(y);
            double cosI_U = cosI*cosU + sinI*sinU;
            double ct = d2z_dy2 * pow( cosI_U, 3);
            obl_pwr_t = ct*(n_after*cosI_prime - n_before*cosI);
        }



        double z1 = ray.z(i);
        double z2 = ray.z(i+1);
        double d = path[i].gap->thi();

        double D = (d + z2 - z1)/cosU_prime;


        s_after = n_after/(n_before/s_before + obl_pwr_s);
        t_after = n_after*cosI_prime*cosI_prime / ( (n_before*cosI*cosI/t_before) + obl_pwr_t );


        s_before = s_after - D;
        t_before = t_after - D;

    }

    // Closing Equation
    double z = ray.z(ray.size()-1-1);
    double zs = s_after*cosU_prime + z - l_prime;
    double zt = t_after*cosU_prime + z - l_prime;

    s_t(0) = zs;
    s_t(1) = zt;

    return s_t;
}


Eigen::Vector2d Trace::aim_chief_ray(const OpticalModel& opt_model, const Field& fld, double wvl)
{
    auto seq_model = opt_model.seq_model();
    if(wvl == 0.0){
        //wvl = seq_model->central_wavelength();
        wvl = opt_model.optical_spec()->spectral_region()->reference_wvl();
    }
    int stop = seq_model->stop_surface();
    Eigen::Vector2d xy_target({0.0,0.0});
    auto aim_pt = iterate_ray(opt_model, stop, xy_target, fld, wvl);

    return aim_pt;
}

Eigen::Vector2d Trace::iterate_ray(const OpticalModel& opt_model, int ifcx, Eigen::Vector2d xy_target, const Field& fld, double wvl)
{
    auto seq_model = opt_model.seq_model();
    auto osp = opt_model.optical_spec();
    //auto fod = osp->paraxial_data().fod;
    FirstOrderData fod = opt_model.paraxial_model()->first_order_data();
    double dist = fod.obj_dist + fod.enp_dist;
    auto pt0 = osp->obj_coord(fld);

    Eigen::Vector2d start_coords;

    if(ifcx > 0)
    {
        if(pt0(0) == 0.0 && xy_target(0) == 0.0)
        {
            // do 1D iteration if field and target points are zero in x
            double start_y;

            auto y_target = xy_target(1);

            // newton
            double x1,x2,y1,y2;
            double diff_x;
            double next_x;
            double delta = 1.0e-5;
            double error = 1.0e-5;

            x1 = 0.0; // initial point
            int cnt = 0;
            try {
                while(true){
                    cnt++;
                    x2 = x1 + delta;
                    y1 = y_stop_coordinate(x1, *seq_model, ifcx, pt0,dist, wvl, y_target);
                    y2 = y_stop_coordinate(x2, *seq_model, ifcx, pt0,dist, wvl, y_target);

                    diff_x = (y2-y1)/(x2-x1);
                    next_x = x1 - y1/diff_x;

                    if((fabs(y1 - 0) < error) || cnt > 30 ) break;

                    x1 = next_x;
                }

                start_y = x1;

            }  catch (TraceError) {
                start_y = 0.0;
            }

            start_coords(0) = 0.0;
            start_coords(1) = start_y;
        }
        else{
            // do 2D iteration. epsfcn is a parameter increment, make proportional to pupil radius

        }
    }
    else{
        // floating stop surface - use entrance pupil for aiming
        start_coords = xy_target;

    }

    return start_coords;
}

void Trace::apply_paraxial_vignetting(const OpticalModel& opt_model)
{
    auto osp = opt_model.optical_spec();
    auto pm = opt_model.paraxial_model();
    double max_field = osp->field_of_view()->max_field();

    for(int j = 0; j < osp->field_of_view()->field_count(); j++){
        auto fld = osp->field_of_view()->field(j);
        double rel_fov = sqrt(fld->x()*fld->x() + fld->y()*fld->y())/max_field;
        //auto vg = pm->paraxial_vignetting(rel_fov);
        //fld->set_vly(1.0 - vg.vly);
        //fld->set_vuy(1.0 - vg.vuy);
    }
}


double Trace::y_stop_coordinate(double y1, const SequentialModel& seq_model, int ifcx, Eigen::Vector3d pt0, double dist, double wvl, double y_target)
{
    Eigen::Vector3d pt1({0.0, y1, dist});
    Eigen::Vector3d dir0 = pt1 - pt0;
    double length = dir0.norm();
    dir0 = dir0/length;

    //std::vector<RayAtSurface> ray;
    std::vector<RayAtSurface> ray_at_srfs;
    Ray ray_trace_result;
    try {
        //RayTraceResult ray_tr_rslt = RayTrace::trace(seq_model, pt0, dir0, wvl);
        ray_trace_result = Trace::trace_ray_from_object(seq_model, pt0, dir0, wvl);
        //ray_at_srfs = ray_tr_rslt.rays;
    }
    catch (TraceMissedSurfaceError& ray_miss) {
        ray_at_srfs = ray_miss.ray_;
        if(ray_miss.surf_ <= ifcx){
            throw ray_miss;
        }
    }
    catch(TraceTIRError& ray_tir){
        ray_at_srfs = ray_tir.ray_;
        if(ray_tir.surf_ < ifcx){
            throw ray_tir;
        }
    }

    //double y_ray = ray[ifcx].pt(1);

    double y_ray = ray_trace_result.at(ifcx).intersect_pt(1);
    //double y_ray = ray_at_srfs[ifcx].intersect_pt(1);

    return (y_ray - y_target);

}


Eigen::Vector3d Trace::bend(Eigen::Vector3d d_in, Eigen::Vector3d normal, double n_in, double n_out)
{
    try {
        auto normal_len = normal.norm();
        auto cosI = d_in.dot(normal)/normal_len;
        auto sinI_sqr = 1.0 - cosI*cosI;
        auto n_cosIp = sqrt(n_out*n_out - n_in*n_in*sinI_sqr) * cosI/abs(cosI);
        auto alpha = n_cosIp - n_in*cosI;
        Eigen::Vector3d d_out = (n_in*d_in + alpha*normal)/n_out;
        return d_out;
    }  catch (...) {
        throw TraceTIRError(d_in, normal, n_in, n_out);
    }
}


Eigen::Vector3d Trace::reflect(Eigen::Vector3d d_in, Eigen::Vector3d normal)
{
    auto normal_len = normal.norm();
    auto cosI = d_in.dot(normal)/normal_len;
    auto d_out = d_in - 2.0*cosI*normal;

    return d_out;
}

Ray Trace::trace_ray_from_object(const SequentialModel& seq_model, Eigen::Vector3d pt0, Eigen::Vector3d dir0, double wvl)
{
    auto path = seq_model.path(wvl);
    return trace_ray_from_object(path, pt0, dir0, wvl);
}


Ray Trace::trace_ray_from_object(Path path, Eigen::Vector3d pt0, Eigen::Vector3d dir0, double wvl, double eps)
{

    Ray ray_trace_result;

    int first_surf = 1;
    int last_surf = path.size()-2;

    // trace object surface
    auto path_itr = path.begin();
    int path_index = 0;
    auto obj = *path_itr;
    auto srf_obj = obj.srf;
    /*
    IntersectPointAndDistance p_and_d = srf_obj->intersect(pt0, dir0);
    auto dst_b4 = p_and_d.distance;
    auto pt_obj = p_and_d.intersect_point;
    */
    double dst_b4;
    Eigen::Vector3d pt_obj;
    srf_obj->intersect(pt_obj, dst_b4, pt0, dir0);

    auto before = obj;
    auto before_pt = pt_obj;
    auto before_dir = dir0;
    auto before_normal = srf_obj->normal(before_pt);
    auto tfrm_from_before = before.tfrm;
    auto z_dir_before = before.z_dir;

    double op_delta = 0.0;
    double opl = 0.0;
    double opl_eic = 0.0;
    int surf = 0;
    RayAtSurface ray_at_srf;
    ray_trace_result.set_status(RayStatus::PassThrough); //as default

    // loop of remaining surfaces in path
    while(true)
    {
        path_itr++;
        path_index++;

        double pp_dst;
        Eigen::Vector3d inc_pt;
        Eigen::Vector3d normal;
        Eigen::Vector3d after_dir;

        try {
            auto after = *path_itr;

            Eigen::Matrix3d rt = tfrm_from_before.rotation;
            Eigen::Vector3d t = tfrm_from_before.transfer;

            Eigen::Vector3d b4_pt  = rt*(before_pt - t);
            Eigen::Vector3d b4_dir = rt*(before_dir);

                     pp_dst = -b4_pt.dot(b4_dir);
            Eigen::Vector3d pp_pt_before = b4_pt + pp_dst*b4_dir;

            auto ifc = after.srf;
            double z_dir_after = after.z_dir;

            // intersect ray with profile
            double pp_dst_intrsct;
            ifc->intersect(inc_pt, pp_dst_intrsct, pp_pt_before, b4_dir, eps, z_dir_before);

            dst_b4 = pp_dst + pp_dst_intrsct;

            ray_at_srf.intersect_pt = before_pt;
            ray_at_srf.after_dir = before_dir;
            ray_at_srf.after_dist = dst_b4;
            ray_at_srf.normal = before_normal;

            //ray_at_srfs.push_back(ray_at_srf);
            ray_trace_result.append(ray_at_srf);

            if(in_surface_range(surf,first_surf,last_surf)){
                opl += before.rndx * dst_b4;
            }

            normal = ifc->normal(inc_pt);


            // refract or reflect ray at interface

            if(ifc->interact_mode() == "reflect"){
                after_dir = reflect(b4_dir,normal);
            }
            else if(ifc->interact_mode() == "transmit"){
                after_dir = bend(b4_dir, normal, before.rndx, after.rndx);
            }
            else{
                after_dir = b4_dir;
            }

            surf += 1;

            /*
             * # Per `Hopkins, 1981 <https://dx.doi.org/10.1080/713820605>`_, the
            #  propagation direction is given by the direction cosines of the
            #  ray and therefore doesn't require the use of a negated
            #  refractive index following a reflection. Thus we use the
            #  (positive) refractive indices from the seq_model.rndx array.
             */

            before_pt = inc_pt;
            before_normal = normal;
            before_dir = after_dir;
            z_dir_before = z_dir_after;
            before = after;
            tfrm_from_before = before.tfrm;

        }
        catch (TraceMissedSurfaceError& ray_miss) {
            ray_at_srf.intersect_pt = before_pt;
            ray_at_srf.after_dir = before_dir;
            ray_at_srf.after_dist = pp_dst;
            ray_at_srf.normal = before_normal;
            //ray_at_srfs.push_back(ray_at_srf);
            ray_trace_result.append(ray_at_srf);
            ray_trace_result.set_status(RayStatus::MissedSurface);
            //throw ray_miss;
            break;
        }
        catch(TraceTIRError& ray_tir){
            ray_at_srf.intersect_pt = inc_pt;
            ray_at_srf.after_dir = before_dir;
            ray_at_srf.after_dist = 0.0;
            ray_at_srf.normal = normal;
            //ray_at_srfs.push_back(ray_at_srf);
            ray_trace_result.append(ray_at_srf);
            ray_trace_result.set_status(RayStatus::TotalReflection);

            throw ray_tir;
        }
        catch(TraceBlockedByApertureError& ray_blocked){
            ray_trace_result.set_status(RayStatus::Blocked);

            ray_at_srf.intersect_pt = before_pt;
            ray_at_srf.after_dir = before_dir;
            ray_at_srf.after_dist = dst_b4;
            ray_at_srf.normal = before_normal;
            ray_trace_result.append(ray_at_srf);

            ray_at_srf.intersect_pt = inc_pt;
            ray_at_srf.after_dir = after_dir;
            ray_at_srf.after_dist = 0.0;
            ray_at_srf.normal = normal;
            ray_trace_result.append(ray_at_srf);
            // continue to trace
            //break;
        }

        if(path_index == (int)path.size()-1){ // stop iteration
            RayAtSurface ray_at_srf;
            ray_at_srf.intersect_pt = inc_pt;
            ray_at_srf.after_dir = after_dir;
            ray_at_srf.after_dist = 0.0;
            ray_at_srf.normal = normal;
            ray_trace_result.append(ray_at_srf);
            //ray_trace_result.set_status(RayStatus::PassThrough);

            op_delta += opl;
            break;
        }
    }


    ray_trace_result.set_op_delta(op_delta);
    ray_trace_result.set_wvl(wvl);


    return ray_trace_result;

}


bool Trace::in_surface_range(int s, int first_surf, int last_surf, bool include_last_surf)
{
    if(first_surf == last_surf){
        return false;
    }
    if(s < first_surf){
        return false;
    }
    if(last_surf == 0){ // assume not input
        return true;
    }
    else{
        if(include_last_surf){
            return (s <= last_surf);
        }
        else{
            return (s < last_surf);
        }
    }

}

#include "trace.h"
#include "trace_error.h"

#include "optical_model.h"
#include "sequential_model.h"
#include "paraxial_model.h"
#include "optical_spec.h"
#include "pupil_spec.h"
#include "field_spec.h"

#include "surface.h"
#include "gap.h"
#include "aperture.h"

using namespace geopter;

Trace::Trace()
{

}

Ray Trace::trace_base(const OpticalModel& opt_model, PupilCrd pupil, const Field& fld, double wvl)
{
    auto vig_pupil = fld.apply_vignetting(pupil);
    auto osp = opt_model.optical_spec();
    //auto fod = osp->paraxial_data().fod;
    opt_model.paraxial_model()->compute_first_order();
    auto fod = opt_model.paraxial_model()->first_order_data();
    double eprad = fod.enp_radius;
    PupilCrd aim_pt;
    aim_pt(0) = 0.0;
    aim_pt(1) = 0.0;
    aim_pt = fld.aim_pt();

    Eigen::Vector3d pt1;
    pt1(0) = eprad*vig_pupil(0) + aim_pt(0);
    pt1(1) = eprad*vig_pupil(1) + aim_pt(1);
    pt1(2) = fod.obj_dist + fod.enp_dist;

    Eigen::Vector3d pt0 = osp->obj_coord(fld);
    Eigen::Vector3d dir0 = pt1 - pt0;
    double length = dir0.norm();

    dir0 = dir0/length;
    //dir0.normalize();

    return Trace::trace(*opt_model.seq_model(), pt0, dir0, wvl);

}

Ray Trace::trace_base(const OpticalModel& opt_model, Eigen::Vector2d pupil, int fld_idx, double wvl)
{
    auto fld = opt_model.optical_spec()->field_of_view()->field(fld_idx);
    return trace_base(opt_model, pupil, *fld, wvl);
}






Eigen::Vector2d Trace::aim_chief_ray(const OpticalModel& opt_model, const Field& fld, double wvl)
{
    auto seq_model = opt_model.seq_model();
    if(wvl == 0.0){
        wvl = seq_model->central_wavelength();
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
        ray_trace_result = Trace::trace(seq_model, pt0, dir0, wvl);
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

Ray Trace::trace(const SequentialModel& seq_model, Eigen::Vector3d pt0, Eigen::Vector3d dir0, double wvl)
{
    auto path = seq_model.path(wvl);
    return trace_raw(path, pt0, dir0, wvl);
}


Ray Trace::trace_raw(Path path, Eigen::Vector3d pt0, Eigen::Vector3d dir0, double wvl, double eps)
{
    //std::vector<RayAtSurface> ray_at_srfs;

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

            auto rt = tfrm_from_before.rotation;
            auto t = tfrm_from_before.transfer;

            Eigen::Vector3d b4_pt  = rt*(before_pt - t);
            Eigen::Vector3d b4_dir = rt*(before_dir);

                     pp_dst = -b4_pt.dot(b4_dir);
            Eigen::Vector3d pp_pt_before = b4_pt + pp_dst*b4_dir;

            auto ifc = after.srf;
            auto z_dir_after = after.z_dir;

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

            ray_at_srf.intersect_pt = inc_pt;
            ray_at_srf.after_dir = after_dir;
            ray_at_srf.after_dist = 0.0;
            ray_at_srf.normal = normal;
            ray_trace_result.append(ray_at_srf);
            break;
        }

        if(path_index == (int)path.size()-1){ // stop iteration
            RayAtSurface ray_at_srf;
            ray_at_srf.intersect_pt = inc_pt;
            ray_at_srf.after_dir = after_dir;
            ray_at_srf.after_dist = 0.0;
            ray_at_srf.normal = normal;
            ray_trace_result.append(ray_at_srf);
            ray_trace_result.set_status(RayStatus::PassThrough);

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

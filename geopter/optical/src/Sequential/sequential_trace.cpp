#define _USE_MATH_DEFINES
#include <cmath>

#include "Sequential/sequential_trace.h"

#include "Sequential/sequential_path.h"
#include "Sequential/trace_error.h"
#include "Spec/optical_spec.h"
#include "Assembly/optical_assembly.h"
#include "Paraxial/first_order.h"
#include "Paraxial/paraxial_ray.h"
#include "Paraxial/paraxial_trace.h"

using namespace geopter;

SequentialTrace::SequentialTrace(OpticalSystem* sys):
    opt_sys_(sys)
{
    num_flds_ = opt_sys_->optical_spec()->field_of_view()->field_count();

    num_wvls_    = opt_sys_->optical_spec()->spectral_region()->wvl_count();
    ref_wvl_idx_ = opt_sys_->optical_spec()->spectral_region()->reference_index();
    ref_wvl_val_ = opt_sys_->optical_spec()->spectral_region()->reference_wvl();

    num_srfs_ = opt_sys_->optical_assembly()->surface_count();
    num_gaps_ = opt_sys_->optical_assembly()->gap_count();
    image_index_ = opt_sys_->optical_assembly()->image_index();
    object_distance_ = opt_sys_->optical_assembly()->gap(0)->thi();

    do_aperture_check_ = false;
    do_apply_vig_ = true;
}

SequentialTrace::~SequentialTrace()
{
    opt_sys_ = nullptr;
}


std::shared_ptr<Ray> SequentialTrace::trace_pupil_ray(const Eigen::Vector2d& pupil_crd, const Field *fld, double wvl)
{
    Eigen::Vector2d vig_pupil = pupil_crd;

    if(do_apply_vig_){
        vig_pupil = fld->apply_vignetting(pupil_crd);
    }

    auto fod = opt_sys_->first_order_data();
    double eprad = fod.enp_radius;
    PupilCrd aim_pt = fld->aim_pt();

    Eigen::Vector3d pt0 = this->object_coord(fld);

    Eigen::Vector3d pt1;
    pt1(0) = eprad*vig_pupil(0) + aim_pt(0);
    pt1(1) = eprad*vig_pupil(1) + aim_pt(1);
    pt1(2) = fod.obj_dist + fod.enp_dist;

    Eigen::Vector3d dir0 = pt1 - pt0;
    dir0.normalize();

    SequentialPath path = sequential_path(0, image_index_, wvl);

    std::shared_ptr<Ray> ray = trace_ray_throughout_path(path, pt0, dir0);
    ray->set_wvl(wvl);

    return ray;
}

int SequentialTrace::trace_pupil_pattern_rays(std::vector< std::shared_ptr<Ray> >& rays, const std::vector<Eigen::Vector2d>& pupils, const Field* fld, double wvl)
{


    auto fod = opt_sys_->first_order_data();
    double eprad = fod.enp_radius;
    PupilCrd aim_pt = fld->aim_pt();

    //Eigen::Vector3d pt0 = this->object_coord(fld);
    Eigen::Vector3d pt0 = fld->object_coord();

    SequentialPath path = sequential_path(0, image_index_, wvl);

    rays.clear();
    rays.reserve(pupils.size());

    int valid_count = 0;

    for(auto& pupil : pupils)
    {
        Eigen::Vector2d vig_pupil = pupil;

        if(do_apply_vig_){
            vig_pupil = fld->apply_vignetting(pupil);
        }

        Eigen::Vector3d pt1;
        pt1(0) = eprad*vig_pupil(0) + aim_pt(0);
        pt1(1) = eprad*vig_pupil(1) + aim_pt(1);
        pt1(2) = fod.obj_dist + fod.enp_dist;

        Eigen::Vector3d dir0 = pt1 - pt0;
        dir0.normalize();

        auto ray = trace_ray_throughout_path(path, pt0, dir0);
        ray->set_wvl(wvl);

        if(ray->status() == PassThrough){
            valid_count++;
        }

        rays.push_back(ray);
    }
    return valid_count;
}

std::shared_ptr<Ray> SequentialTrace::trace_ray_throughout_path(const SequentialPath& seq_path, const Eigen::Vector3d& pt0, const Eigen::Vector3d& dir0)
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

    constexpr double eps = 1.0e-5;
    constexpr double z_dir = 1.0; // used for reflection, not yet implemented


    // first surface
    auto ray_at_srf = std::make_shared<RayAtSurface>();
    ray_at_srf->intersect_pt         = pt0;
    ray_at_srf->distance_from_before = 0.0;
    ray_at_srf->after_dir            = dir0;
    ray_at_srf->normal               = seq_path.at(0).srf->normal(pt0);
    ray_at_srf->optical_path_length  = 0.0;

    ray->append(ray_at_srf);

    // trace ray till the image
    int path_size = seq_path.size();

    if (path_size == 1){
        return ray;
    }

    try {
        for(int i = 1; i < path_size; i++) {

            Eigen::Matrix3d rt = transformation_from_before.rotation;
            Eigen::Vector3d t  = transformation_from_before.transfer;

            Eigen::Vector3d rel_before_pt = rt*(before_pt - t); // relative source point looked from current surface
            Eigen::Vector3d rel_before_dir = rt*before_dir;     // relative ray direction looked from current surface

            double dist_from_before_to_perpendicular = -rel_before_pt.dot(rel_before_dir); // distance from previous point to foot of perpendicular
            Eigen::Vector3d foot_of_perpendicular_pt = rel_before_pt + dist_from_before_to_perpendicular*rel_before_dir; // foot of perpendicular from the current surface apex to the incident ray line

            Surface* cur_srf = seq_path.at(i).srf;
            double dist_from_perpendicular_to_intersect_pt; // distance from the foot of perpendicular to the intersect point
            cur_srf->intersect(intersect_pt, dist_from_perpendicular_to_intersect_pt, foot_of_perpendicular_pt, rel_before_dir, eps, z_dir);
            Eigen::Vector3d srf_normal = cur_srf->normal(intersect_pt); // surface normal at the intersect point

            distance_from_before = dist_from_before_to_perpendicular + dist_from_perpendicular_to_intersect_pt; // distance between before and current intersect point

            n_out = seq_path.at(i).rind;
            after_dir = bend(before_dir, srf_normal, n_in, n_out);

            ray_at_srf = std::make_shared<RayAtSurface>();
            ray_at_srf->intersect_pt         = intersect_pt;
            ray_at_srf->after_dir            = after_dir.normalized();
            ray_at_srf->distance_from_before = distance_from_before;
            ray_at_srf->optical_path_length  = n_in*distance_from_before;
            ray_at_srf->normal               = srf_normal;

            ray->append(ray_at_srf);

            if(do_aperture_check_) {
                if( !cur_srf->point_inside(intersect_pt(0),intersect_pt(1)) ){
                    ray->set_status(RayStatus::Blocked);
                    return ray;
                    break;
                }
            }

            before_pt  = intersect_pt;
            before_dir = after_dir;
            n_in       = n_out;
            transformation_from_before = cur_srf->local_transform();
        }

    } catch (TraceMissedSurfaceError& ray_miss) {
        ray->set_status(RayStatus::MissedSurface);
        return ray;
    } catch (TraceTIRError& ray_tir){
        ray->set_status(RayStatus::TotalReflection);
        return ray;
    }

    return ray;
}


Eigen::Vector2d SequentialTrace::trace_coddington(const Field *fld, double wvl)
{
    /*
         * R. Kingslake, "Lens Design Fundamentals", p292
         */

        Eigen::Vector2d s_t;

        SequentialPath path = overall_sequential_path(wvl);

        // on axis
        int num_srf = opt_sys_->optical_assembly()->surface_count();
        int img_srf = num_srf-1;
        int last_surf = num_srf - 1 -1;

        ParaxialTrace *parax_tracer = new ParaxialTrace(opt_sys_);

        double y0 = opt_sys_->axial_ray(ref_wvl_idx_)->at(0)->y();
        double u0 = opt_sys_->axial_ray(ref_wvl_idx_)->at(0)->u();
        auto ax_ray = parax_tracer->trace_paraxial_ray_from_object(y0, u0, wvl);

        double y = ax_ray->at(last_surf)->y();
        double u_prime = ax_ray->at(img_srf)->u_prime();
        double l_prime = -y/u_prime;

        delete parax_tracer;


        // Here, we have l_prime

        // off axis
        std::shared_ptr<Ray> ray = trace_pupil_ray(PupilCrd({0.0, 0.0}), fld, wvl);

        double s_before, t_before, s_after, t_after;
        double n_before, n_after;
        double obl_pwr_s, obl_pwr_t;
        double sinI, sinI_prime, cosI, cosI_prime, sinU, sinU_prime, cosU, cosU_prime;


        // Opening Equation
        if(std::isinf(path.at(0).d)){
            s_before = std::numeric_limits<double>::infinity();
            t_before = std::numeric_limits<double>::infinity();
        }else{
            auto dir0 = ray->at(0)->after_dir;
            double cosUpr = dir0(2);
            //double B = opt_sys_->optical_assembly()->gap(0)->thi();
            double B = object_distance_;
            double Zpr = ray->z(1);

            s_before = (B - Zpr)/cosUpr;
            t_before = s_before;
        }


        for(int i = 1; i < ray->size()-1; i++) {
            Surface* surf = path.at(i).srf;

            n_before = path.at(i-1).rind;
            n_after = path.at(i).rind;
            cosI = cos(ray->aoi(i));
            cosI_prime = cos(ray->aor(i));
            //sinI = sqrt(1.0 - cosI*cosI);
            sinI = sin(ray->aoi(i));
            sinI_prime = sinI * n_before/n_after;
            //cosI_prime = sqrt(1.0 - sinI_prime*sinI_prime);

            cosU = ray->at(i-1)->after_dir(2);//ray.at(i-1).after_dir.norm();
            sinU = sqrt(1.0 - cosU*cosU);
            cosU_prime = ray->at(i)->after_dir(2);//ray.at(i).after_dir.norm();

            if(surf->profile()->name() == "SPH") {
                double c = surf->profile()->cv();
                obl_pwr_s = c*(n_after*cosI_prime - n_before*cosI);
                obl_pwr_t = obl_pwr_s;
            }else{ // aspherical
                double sinI_U = sinI*cosU - cosI*sinU;
                double y = ray->y(i);
                double cs = sinI_U/y;
                obl_pwr_s = cs*(n_after*cosI_prime - n_before*cosI);

                double d2z_dy2 = surf->profile()->deriv_2nd(y);
                double cosI_U = cosI*cosU + sinI*sinU;
                double ct = d2z_dy2 * pow( cosI_U, 3);
                obl_pwr_t = ct*(n_after*cosI_prime - n_before*cosI);
            }


            double z1 = ray->z(i);
            double z2 = ray->z(i+1);
            double d = path.at(i).d;

            double D = (d + z2 - z1)/cosU_prime;

            s_after = n_after/(n_before/s_before + obl_pwr_s);
            t_after = n_after*cosI_prime*cosI_prime / ( (n_before*cosI*cosI/t_before) + obl_pwr_t );


            s_before = s_after - D;
            t_before = t_after - D;

        }

        // Closing Equation
        double z = ray->z(ray->size()-1-1);
        double zs = s_after*cosU_prime + z - l_prime;
        double zt = t_after*cosU_prime + z - l_prime;

        s_t(0) = zs;
        s_t(1) = zt;

        return s_t;
}


SequentialPath SequentialTrace::overall_sequential_path(double wvl)
{
    int start = 0;
    int end = opt_sys_->optical_assembly()->image_index();
    return sequential_path(start, end, wvl);
}


SequentialPath SequentialTrace::sequential_path(int start, int end, double wvl)
{
    assert(end <= num_srfs_);

    SequentialPath path;
    SequentialPathComponent path_comp;

    for(int i = start; i <= end; i++)
    {
        if ( i < num_srfs_ ) {
            path_comp.srf = opt_sys_->optical_assembly()->surface(i);
        }else{
            path_comp.srf = nullptr;
        }

        if( i < num_gaps_ ) {
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
    Eigen::Vector2d aim_pt = search_aim_point(stop, xy_target, fld, wvl);

    return aim_pt;
}


Eigen::Vector2d SequentialTrace::search_aim_point(int srf_idx, const Eigen::Vector2d& xy_target, const Field *fld, double wvl)
{
    assert(srf_idx > 0);

    double enp_dist = opt_sys_->first_order_data().enp_dist;
    double obj_dist = opt_sys_->optical_assembly()->gap(0)->thi();
    double obj2enp_dist = obj_dist + enp_dist;

    Eigen::Vector3d pt0 = this->object_coord(fld);

    Eigen::Vector2d start_coords;
    double start_y;
    double y_target = xy_target(1);

    SequentialPath path = sequential_path(0, srf_idx, wvl);
    Ray ray;
    

    // newton
    double x1,x2,y1,y2;
    double diff_x;
    double next_x;
    constexpr double delta = 1.0e-5;
    constexpr double error = 1.0e-5;

    x1 = 0.0;
    int cnt = 0;

    while(true) {
        cnt++;
        x2 = x1 + delta;

        y1 = y_stop_coordinate(x1,srf_idx,pt0, obj2enp_dist, wvl, y_target);
        y2 = y_stop_coordinate(x2,srf_idx,pt0, obj2enp_dist, wvl, y_target);

        diff_x = (y2 - y1) / (x2 - x1);
        next_x = x1 - y1/diff_x;

        if(fabs(y1 - y_target) < error || cnt > 30) {
            break;
        }

        x1 = next_x;
    }

    start_y = x1;

    start_coords(0) = 0.0;
    start_coords(1) = start_y;

    return start_coords;

}

double SequentialTrace::y_stop_coordinate(double y1, int ifcx, const Eigen::Vector3d& pt0, double dist, double wvl, double y_target)
{
    Eigen::Vector3d pt1({0.0, y1, dist});
    Eigen::Vector3d dir0 = pt1 - pt0;
    double length = dir0.norm();
    dir0 = dir0/length;

    //std::vector<RayAtSurface> ray;
    std::vector<RayAtSurface> ray_at_srfs;
    std::shared_ptr<Ray> ray_trace_result;
    try {
        //RayTraceResult ray_tr_rslt = RayTrace::trace(seq_model, pt0, dir0, wvl);
        ray_trace_result = trace_ray_throughout_path(overall_sequential_path(wvl), pt0, dir0);
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


    double y_ray = ray_trace_result->at(ifcx)->intersect_pt(1);
    //double y_ray = ray_at_srfs[ifcx].intersect_pt(1);

    return (y_ray - y_target);


}

Eigen::Vector3d SequentialTrace::bend(const Eigen::Vector3d& d_in, const Eigen::Vector3d& normal, double n_in, double n_out)
{
    try {
        double normal_len = normal.norm();
        double cosI = d_in.dot(normal)/normal_len;
        double sinI_sqr = 1.0 - cosI*cosI;
        double n_cosIp = sqrt(n_out*n_out - n_in*n_in*sinI_sqr) * cosI/abs(cosI);
        double alpha = n_cosIp - n_in*cosI;
        Eigen::Vector3d d_out = (n_in*d_in + alpha*normal)/n_out;
        return d_out;
    }  catch (...) {
        throw TraceTIRError(d_in, normal, n_in, n_out);
    }
}


Eigen::Vector3d SequentialTrace::object_coord(const Field* fld)
{
    Eigen::Vector3d obj_pt;

    auto fod = opt_sys_->first_order_data();

    Eigen::Vector3d ang_dg;
    Eigen::Vector3d img_pt;
    Eigen::Vector3d dir_tan;

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
        obj_pt = -dir_tan*(fod.obj_dist + fod.enp_dist);
        break;

    case FieldType::OBJ_HT:
        obj_pt(0) = fld_x;
        obj_pt(1) = fld_y;
        obj_pt(2) = 0.0;
        break;

    case FieldType::IMG_HT:
        img_pt = Eigen::Vector3d({fld_x, fld_y, 0.0});
        obj_pt = fod.red*img_pt;
        break;

    default:
        obj_pt = Eigen::Vector3d::Zero(3);
    }

    return obj_pt;
}


std::vector<double> SequentialTrace::compute_vignetting_factors(const Field& fld)
{
    double vuy = compute_vignetting_factor_for_pupil(Eigen::Vector2d({0.0, 1.0}), fld);
    double vly = compute_vignetting_factor_for_pupil(Eigen::Vector2d({0.0, -1.0}), fld);
    double vux = compute_vignetting_factor_for_pupil(Eigen::Vector2d({1.0, 0.0}), fld);
    double vlx = compute_vignetting_factor_for_pupil(Eigen::Vector2d({-1.0, 0.0}), fld);

    std::vector<double> vig_factors = std::vector<double>( {vuy, vly, vux, vlx});

    return vig_factors;
}


double SequentialTrace::compute_vignetting_factor_for_pupil(const Eigen::Vector2d& full_pupil, const Field &fld)
{
    bool orig_vig_state = do_apply_vig_;
    do_apply_vig_ = false;

    bool orig_aperture_check = do_aperture_check_;
    do_aperture_check_ = true;

    Eigen::Vector2d vig_pupil = full_pupil;

    double vig = 0.0;

    double a = 0.0;
    double b = 1.0;

    vig_pupil(0) = full_pupil(0)*(1.0 - a);
    vig_pupil(1) = full_pupil(1)*(1.0 - a);
    std::shared_ptr<Ray> ray_full_marginal = trace_pupil_ray(vig_pupil, &fld, ref_wvl_val_);

    if(ray_full_marginal->status() == RayStatus::PassThrough){ // no vignetting
        return a; // 0.0
    }

    constexpr double eps = 1.0e-5;
    constexpr int max_loop_cnt = 30;

    double m;
    int loop_cnt = 0;

    while(true){
        loop_cnt++;

        m = (a+b)/2.0;
        vig_pupil(0) = full_pupil(0)*(1.0 - m);
        vig_pupil(1) = full_pupil(1)*(1.0 - m);
        std::shared_ptr<Ray> ray_m = trace_pupil_ray(vig_pupil, &fld, ref_wvl_val_);

        if(ray_m->status() == RayStatus::PassThrough){
            b = m;
        }else{
            a = m;
        }

        if(abs(a-b) < eps  || loop_cnt > max_loop_cnt){
            break;
        }

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

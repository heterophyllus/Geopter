
#include "analysis/wave_aberration.h"

#include <iostream>

#include "sequential/sequential_trace.h"
#include "assembly/optical_assembly.h"

using namespace geopter;

WaveAberration::WaveAberration(OpticalSystem* opt_sys) :
    opt_sys_(opt_sys)
{

}

WaveAberration::~WaveAberration()
{
    opt_sys_ = nullptr;
}

double WaveAberration::wave_abr_full_calc(const std::shared_ptr<Ray>& ray, const std::shared_ptr<Ray>& chief_ray, const Field* fld, ReferenceSphere& ref_sphere)
{
    int k = opt_sys_->optical_assembly()->image_index() - 1;
    Surface* srf = opt_sys_->optical_assembly()->surface(k);
    //double exp_dist_parax = opt_sys_->paraxial_data()->exit_pupil_distance();

    double cr_exp_dist;
    Eigen::Vector3d cr_exp_pt;
    get_chief_ray_exp_segment(cr_exp_pt, cr_exp_dist, chief_ray);

    Eigen::Vector3d ref_dir = ref_sphere.ref_dir();
    double ref_sphere_radius = ref_sphere.radius();

    double e1 = eic_distance(ray->at(1)->intersect_pt(), ray->at(0)->after_dir(),
                             chief_ray->at(1)->intersect_pt(), chief_ray->at(0)->after_dir());

    double ekp = eic_distance(ray->at(k)->intersect_pt(), ray->at(k)->after_dir(),
                              chief_ray->at(k)->intersect_pt(), chief_ray->at(k)->after_dir());

    Eigen::Vector3d ray_inc_pt_before_img;
    Eigen::Vector3d ray_dir_before_img;
    transform_after_surface(ray_inc_pt_before_img, ray_dir_before_img, srf,ray->at(k));
    double dst = ekp - cr_exp_dist;
    Eigen::Vector3d eic_exp_pt = ray_inc_pt_before_img - dst*ray_dir_before_img;
    Eigen::Vector3d p_coord = eic_exp_pt - cr_exp_pt;

    double F = ref_dir.dot(ray_dir_before_img) - ray_dir_before_img.dot(p_coord)/ref_sphere_radius;
    double J = p_coord.dot(p_coord)/ref_sphere_radius - 2.0*ref_dir.dot(p_coord);

    double soln = ref_dir(2)*chief_ray->back()->after_dir()(2);
    double sign_soln = (soln > 0.0) - (soln < 0.0);
    double denom = F + sign_soln*sqrt( F*F + J/ref_sphere_radius );
    double ep;
    if(fabs(denom) < std::numeric_limits<double>::epsilon()){
        ep = 0.0;
    }else{
        ep = J/denom;
    }

    double ray_op = ray->optical_path_length();
    double chief_ray_op = chief_ray->optical_path_length();

    double wvl = chief_ray->wavelength();
    double n_img = opt_sys_->optical_assembly()->image_space_gap()->material()->rindex(wvl);
    double n_obj = opt_sys_->optical_assembly()->gap(0)->material()->rindex(wvl);

    n_img = fabs(n_img);
    n_obj = fabs(n_obj);

    double opd = -n_obj*e1 - ray_op + n_img*ekp + chief_ray_op - n_img*ep;

    return opd;

}

double WaveAberration::wave_abr_full_calc(const std::shared_ptr<Ray>& ray, const std::shared_ptr<Ray>& chief_ray)
{
    /*
     * 1. calculate chief ray intersection with exit pupil
     * 2. calculate reference sphere
     * 3. calculate opd
     *
     */

    // ---> 1. calculate chief ray intersection with exit pupil

    int k = opt_sys_->optical_assembly()->image_index() - 1;
    Surface* srf = opt_sys_->optical_assembly()->surface(k);
    //double exp_dist_parax = opt_sys_->paraxial_data()->exit_pupil_distance();

    double cr_exp_dist;
    Eigen::Vector3d cr_exp_pt;
    get_chief_ray_exp_segment(cr_exp_pt, cr_exp_dist, chief_ray);

    // <---

    //---> 2. calculate reference sphere

    ReferenceSphere ref_sphere = setup_reference_sphere(chief_ray, cr_exp_pt);

    Eigen::Vector3d ref_dir = ref_sphere.ref_dir();
    double ref_sphere_radius = ref_sphere.radius();

    // <---

    // ---> 3. calculate opd
    double e1 = eic_distance(ray->at(1)->intersect_pt(), ray->at(0)->after_dir(),
                             chief_ray->at(1)->intersect_pt(), chief_ray->at(0)->after_dir());

    double ekp = eic_distance(ray->at(k)->intersect_pt(), ray->at(k)->after_dir(),
                              chief_ray->at(k)->intersect_pt(), chief_ray->at(k)->after_dir());

    Eigen::Vector3d ray_inc_pt_before_img;
    Eigen::Vector3d ray_dir_before_img;
    transform_after_surface(ray_inc_pt_before_img, ray_dir_before_img, srf,ray->at(k));
    double dst = ekp - cr_exp_dist;
    Eigen::Vector3d eic_exp_pt = ray_inc_pt_before_img - dst*ray_dir_before_img;
    Eigen::Vector3d p_coord = eic_exp_pt - cr_exp_pt;

    double F = ref_dir.dot(ray_dir_before_img) - ray_dir_before_img.dot(p_coord)/ref_sphere_radius;
    double J = p_coord.dot(p_coord)/ref_sphere_radius - 2.0*ref_dir.dot(p_coord);

    double soln = ref_dir(2)*chief_ray->back()->after_dir()(2);
    double sign_soln = (soln > 0.0) - (soln < 0.0);
    double denom = F + sign_soln*sqrt( F*F + J/ref_sphere_radius );
    double ep;
    if(fabs(denom) < std::numeric_limits<double>::epsilon()){
        ep = 0.0;
    }else{
        ep = J/denom;
    }

    double ray_op = ray->optical_path_length();
    double chief_ray_op = chief_ray->optical_path_length();

    double ref_wvl_val = opt_sys_->optical_spec()->spectral_region()->reference_wvl();
    double n_img = opt_sys_->optical_assembly()->image_space_gap()->material()->rindex(ref_wvl_val);
    double n_obj = opt_sys_->optical_assembly()->gap(0)->material()->rindex(ref_wvl_val);

    n_img = fabs(n_img);
    n_obj = fabs(n_obj);

    double opd = -n_obj*e1 - ray_op + n_img*ekp + chief_ray_op - n_img*ep;

    return opd;

}

double WaveAberration::eic_distance(const Eigen::Vector3d& p, const Eigen::Vector3d& d, const Eigen::Vector3d& p0, const Eigen::Vector3d& d0)
{
    double e = ( (d + d0).dot(p - p0) ) / ( 1.0 + d.dot(d0) );
    return e;
}



void WaveAberration::transform_after_surface(Eigen::Vector3d& before_pt, Eigen::Vector3d& before_dir, const Surface* srf, const RaySegment* ray_seg)
{
    if(srf->decenter()){
        // get transformation info after surf
        // not implemented yet
        std::cerr << "not implemented: WaveAberration::transform_after_surface()" << std::endl;
    }else{
        before_pt = ray_seg->intersect_pt();
        before_dir = ray_seg->after_dir();
    }
}


ReferenceSphere WaveAberration::setup_reference_sphere(const std::shared_ptr<Ray>& chief_ray, const Eigen::Vector3d& cr_exp_pt, const Eigen::Vector2d& image_pt_2d)
{
    double exp_dist_parax = opt_sys_->first_order_data()->exp_dist;

    double foc = 0.0;
    //double dist = foc / chief_ray->back()->after_dir()(2);
    //Eigen::Vector3d image_pt = chief_ray->back()->intersect_pt() - dist*chief_ray->back()->after_dir();
    Eigen::Vector3d image_pt = Eigen::Vector3d({image_pt_2d(0), image_pt_2d(1), foc});

    Eigen::Vector3d img_pt = image_pt;
    img_pt(2) += opt_sys_->first_order_data()->img_dist;

    Eigen::Vector3d ref_sphere_vec = img_pt - cr_exp_pt;
    double ref_sphere_radius = ref_sphere_vec.norm();
    Eigen::Vector3d ref_dir = ref_sphere_vec.normalized();

    return ReferenceSphere(image_pt, ref_dir, ref_sphere_radius, exp_dist_parax);
}

ReferenceSphere WaveAberration::setup_reference_sphere(const std::shared_ptr<Ray>& chief_ray, const Eigen::Vector3d& cr_exp_pt)
{
    double exp_dist_parax = opt_sys_->first_order_data()->exp_dist;

    double foc = 0.0;
    double dist = foc / chief_ray->back()->after_dir()(2);

    Eigen::Vector3d image_pt = chief_ray->back()->intersect_pt() - dist*chief_ray->back()->after_dir();
    //Eigen::Vector3d image_pt = Eigen::Vector3d({image_pt_2d(0), image_pt_2d(1), foc});

    Eigen::Vector3d img_pt = image_pt;
    img_pt(2) += opt_sys_->first_order_data()->img_dist;

    Eigen::Vector3d ref_sphere_vec = img_pt - cr_exp_pt;
    double ref_sphere_radius = ref_sphere_vec.norm();
    Eigen::Vector3d ref_dir = ref_sphere_vec.normalized();

    return ReferenceSphere(image_pt, ref_dir, ref_sphere_radius, exp_dist_parax);
}

void WaveAberration::get_chief_ray_exp_segment(Eigen::Vector3d& cr_exp_pt, double& cr_exp_dist, const std::shared_ptr<Ray> chief_ray)
{
    int k = opt_sys_->optical_assembly()->image_index() - 1;
    Surface* srf = opt_sys_->optical_assembly()->surface(k);
    double exp_dist_parax = opt_sys_->first_order_data()->exp_dist;

    Eigen::Vector3d cr_inc_pt_before_img;
    Eigen::Vector3d cr_dir_before_img;

    transform_after_surface(cr_inc_pt_before_img, cr_dir_before_img, srf, chief_ray->at(k));

    double h = cr_inc_pt_before_img(1);
    double u = cr_dir_before_img(1);
    constexpr double eps = 1.0e-14;

    if(fabs(u) < eps){
        cr_exp_dist = exp_dist_parax;
    }else{
        cr_exp_dist  = -h/u;
    }

    cr_exp_pt = cr_inc_pt_before_img + cr_exp_dist*cr_dir_before_img;
}


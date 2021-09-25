#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>

#include "Paraxial/paraxial_trace.h"

#include "Spec/optical_spec.h"
#include "Assembly/optical_assembly.h"
#include "Assembly/surface_profile.h"
#include "Material/material.h"

using namespace geopter;

ParaxialTrace::ParaxialTrace(OpticalSystem* sys)
{
    opt_sys_ = sys;
    compute_starting_data();
}

ParaxialTrace::~ParaxialTrace()
{
    opt_sys_ = nullptr;
}

std::shared_ptr<ParaxialRay> ParaxialTrace::trace_paraxial_ray_from_object(double y0, double u0, double wvl)
{
    auto par_ray = std::make_shared<ParaxialRay>();
    int img = opt_sys_->optical_assembly()->image_index();

    ParaxialPath par_path = paraxial_path(0, img, wvl);
    int path_size = par_path.size();

    if(path_size == 0) {
        return par_ray; // empty
    }


    // object surface
    double c0 = par_path.at(0).c;
    double u0_prime = u0;
    double i0 = y0*c0 + u0;
    double n0 = par_path.at(0).n;
    par_ray->append(y0, u0_prime, i0, n0);

    if(path_size == 1) {
        return par_ray;
    }


    // trace the rest
    double d = par_path.at(0).t;
    double y = y0 + d*u0_prime;
    double u = u0_prime;
    double c;
    double i, i_prime;
    double n = n0;
    double n_prime;
    double u_prime;
    
    for(int k = 1; k < path_size; k++) {
        c = par_path.at(k).c;
        n_prime = par_path.at(k).n;
        i = u + y*c;
        i_prime = i*(n/n_prime);
        u_prime = i_prime - y*c;

        par_ray->append(y, u_prime, i, n);

        // transfer to next
        d = par_path.at(k).t;
        y = y + d*u_prime;
        u = u_prime;
        n = n_prime;
    }

    return par_ray;
}

std::shared_ptr<ParaxialRay> ParaxialTrace::trace_paraxial_ray_parallel_to_axis_at_s1()
{
    const double ref_wvl = opt_sys_->optical_spec()->spectral_region()->reference_wvl();
    const int img = opt_sys_->optical_assembly()->image_index();

    ParaxialPath par_path = paraxial_path(0, img, ref_wvl);

    constexpr double y1 = 1.0;
    constexpr double u1 = 0.0;

    auto par_ray = std::make_shared<ParaxialRay>();

    // at object
    double t0 = par_path.at(0).t;
    double y0 = y1 - t0*u1;
    double u0_prime = u1;
    double u0 = u0_prime;
    double c0 = par_path.at(0).c;
    double i0 = u0 + y0*c0;
    double n0 = par_path.at(0).n;

    par_ray = trace_paraxial_ray_from_object(y0, u0, ref_wvl);

    return par_ray;

}

std::shared_ptr<ParaxialRay> ParaxialTrace::trace_paraxial_ray_with_slope_at_s1()
{
    const int img = opt_sys_->optical_assembly()->image_index();

    const double ref_wvl = opt_sys_->optical_spec()->spectral_region()->reference_wvl();
    
    double n0 = opt_sys_->optical_assembly()->gap(0)->material()->rindex(ref_wvl);
    double uq0 = 1.0/n0;
    
    ParaxialPath par_path = paraxial_path(0, img, ref_wvl);

    auto p_ray_bar = std::make_shared<ParaxialRay>();

    double ybar1 = 0.0;
    double ubar1 = uq0;

    // at object
    double ubar0_prime = ubar1;
    double ubar0 = ubar0_prime;
    double t0 = par_path.at(0).t;
    double ybar0 = ybar1 - t0*ubar0_prime;
    double c0 = par_path.at(0).c;
    double i0 = ubar0 + ybar0*c0;
    
    p_ray_bar = trace_paraxial_ray_from_object(ybar0, ubar0, ref_wvl);
    
    return p_ray_bar;
}

ParaxialPath ParaxialTrace::paraxial_path(int start, int end, double wvl)
{
    if(start <= end) {
        return forward_paraxial_path(start, end, wvl);
    } else {
        return reverse_paraxial_path(start, end, wvl);
    }
}

ParaxialPath ParaxialTrace::forward_paraxial_path(int start, int end, double wvl)
{
    assert(start <= end);

    int num_gaps = opt_sys_->optical_assembly()->gap_count();

    ParaxialPathComponent par_path_comp;
    ParaxialPath par_path;

    for(int i = start; i <= end; i++) {
        par_path_comp.c = opt_sys_->optical_assembly()->surface(i)->profile()->cv();

        if( i < num_gaps ){
            par_path_comp.t = opt_sys_->optical_assembly()->gap(i)->thi();
            par_path_comp.n = opt_sys_->optical_assembly()->gap(i)->material()->rindex(wvl);
        }else{
            par_path_comp.t = 0.0;
            par_path_comp.n = 1.0;
        }

        par_path.append(par_path_comp);
    }

    return par_path;
}

ParaxialPath ParaxialTrace::reverse_paraxial_path(int start, int end, double wvl)
{
    assert(start >= end);

    ParaxialPathComponent par_path_comp;
    ParaxialPath par_path;

    for(int i = start; i >= end; i--) {
        par_path_comp.c  = - opt_sys_->optical_assembly()->surface(i)->profile()->cv();

        if( i > 0 ){
            par_path_comp.t = opt_sys_->optical_assembly()->gap(i-1)->thi();
            par_path_comp.n = opt_sys_->optical_assembly()->gap(i-1)->material()->rindex(wvl);
        }else{
            par_path_comp.t = 0.0;
            par_path_comp.n = 1.0;
        }

        par_path.append(par_path_comp);
    }

    return par_path;
}


void ParaxialTrace::compute_starting_data()
{
    double ref_wvl = opt_sys_->optical_spec()->spectral_region()->reference_wvl();

    double n_0 = opt_sys_->optical_assembly()->gap(0)->material()->rindex(ref_wvl);
    double n_k = opt_sys_->optical_assembly()->image_space_gap()->material()->rindex(ref_wvl);


    auto p_ray = trace_paraxial_ray_parallel_to_axis_at_s1();
    auto q_ray = trace_paraxial_ray_with_slope_at_s1();

    double ak1 = p_ray->back()->ht;
    double bk1 = q_ray->back()->ht;
    double ck1 = n_k*p_ray->back()->slp;
    double dk1 = n_k*q_ray->back()->slp;

    int stop = opt_sys_->optical_assembly()->stop_index();

    double n_s = opt_sys_->optical_assembly()->gap(stop)->material()->rindex(ref_wvl);
    double as1 = p_ray->at(stop)->ht;
    double bs1 = q_ray->at(stop)->ht;
    double cs1 = n_s*p_ray->at(stop)->slp;
    double ds1 = n_s*q_ray->at(stop)->slp;

    double ybar1 = -bs1;
    double ubar1 = as1;

    //n_0 = n_0
    double enp_dist = -ybar1/(n_0*ubar1);
    double thi0 = opt_sys_->optical_assembly()->gap(0)->thi();

    double red = dk1 + thi0*ck1;
    double obj2enp_dist = thi0 + enp_dist;

    double y0 = 0.0;
    double u0 = 1.0;
    double uk;

    // get starting [y,u]
    PupilSpec* pupil = opt_sys_->optical_spec()->pupil_spec();
    switch (pupil->pupil_type())
    {
    case PupilType::EPD:
        u0 = 0.5 * pupil->value() / obj2enp_dist;
        break;
    case PupilType::NAO:
        u0 = n_0*tan(asin(pupil->value()/n_0));
        break;
    case PupilType::FNO:
        uk = -1.0/(2.0*pupil->value());
        u0 = uk/red;
        break;
    case PupilType::NA:
        uk = n_k*tan(asin(pupil->value()/n_k));
        u0 = uk/red;
        break;
    default:
        std::cout << "Invalid pupil type" << std::endl;
        break;
    }

    // get starting [ybar, ubar]
    double ybar0 = 1.0;
    double ubar0 = 0.0;

    FieldSpec* fov = opt_sys_->optical_spec()->field_of_view();
    double max_fld = fov->max_field();
    double ang;

    switch(fov->field_type()) {
        case FieldType::OBJ_ANG:
            ang = max_fld * M_PI/180.0;
            ubar0 = tan(ang);
            ybar0 = -ubar0 * obj2enp_dist;
            break;
        case FieldType::OBJ_HT:
            ybar0 = -max_fld;
            ubar0 = -ybar0/obj2enp_dist;
            break;
        case FieldType::IMG_HT:
            ybar0 = red*max_fld;
            ubar0 = -ybar0/obj2enp_dist;
            break;
        default:
            std::cout << "Invalid field type" << std::endl;
            break;
    }

    ref_y0_ = y0;
    ref_u0_ = u0;
    ref_ybar0_ = ybar0;
    ref_ubar0_ = ubar0;
}


void ParaxialTrace::get_starting_coords(double *y0, double *u0, double *ybar0, double *ubar0) const
{
    *y0 = ref_y0_;
    *u0 = ref_u0_;
    *ybar0 = ref_ybar0_;
    *ubar0 = ref_ubar0_;
}

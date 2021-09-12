#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <iomanip>
#include <sstream>

#include "Eigen/LU"

#include "Paraxial/paraxial_model.h"
#include "Paraxial/paraxial_trace.h"

#include "Spec/optical_spec.h"
#include "Spec/wvl_spec.h"
#include "Spec/field_spec.h"
#include "Spec/pupil_spec.h"

#include "System/optical_system.h"

#include "Assembly/optical_assembly.h"
#include "Assembly/surface.h"
#include "Assembly/surface_profile.h"
#include "Assembly/gap.h"

#include "Material/material.h"
#include "Material/glass.h"


using namespace geopter;


ParaxialModel::ParaxialModel(OpticalSystem* sys) :
    opt_sys_(sys)
{
    fod_ = FirstOrderData();
}

ParaxialModel::~ParaxialModel()
{
    ax_rays_.clear();
    pr_rays_.clear();
    opt_sys_ = nullptr;
}


ParaxialRay ParaxialModel::axial_ray(int wi) const
{
    return ax_rays_[wi];
}

ParaxialRay ParaxialModel::principle_ray(int wi) const
{
    return pr_rays_[wi];
}

void ParaxialModel::update_model()
{
    // retrace axial/principle ray
    // recalculate first order data
    compute_first_order(true);
}

FirstOrderData ParaxialModel::first_order_data() const
{
    return fod_;
}


void ParaxialModel::compute_paraxial_rays()
{   
    ParaxialTrace* tracer = new ParaxialTrace(opt_sys_);

    // trace p/q rays
    p_ray_ = tracer->trace_paraxial_ray_parallel_to_axis_at_s1();
    q_ray_ = tracer->trace_paraxial_ray_with_slope_at_s1();


    // trace ax/pr rays
    
    // starting coords are computed with reference wavelength and are common for all wavelength
    double y0, u0, ybar0, ubar0;
    get_starting_coords(&y0, &u0, &ybar0, &ubar0);

    int img = opt_sys_->optical_assembly()->image_index();
    int num_wvls = opt_sys_->optical_spec()->spectral_region()->wvl_count();

    ax_rays_.clear();
    pr_rays_.clear();

    for(int wi = 0; wi < num_wvls; wi++) 
    {
        double wvl = opt_sys_->optical_spec()->spectral_region()->wavelength(wi);
        ParaxialRay ax_ray_for_wvl = tracer->trace_paraxial_ray_from_object(y0, u0, wvl);
        ax_ray_for_wvl.set_name("axial ray traced with W" + std::to_string(wi));

        ParaxialRay pr_ray_for_wvl = tracer->trace_paraxial_ray_from_object(ybar0, ubar0, wvl);
        pr_ray_for_wvl.set_name("principle ray traced with W" + std::to_string(wi));

        ax_rays_.push_back(ax_ray_for_wvl);
        pr_rays_.push_back(pr_ray_for_wvl);
    }

    delete tracer;
}


void ParaxialModel::compute_first_order(bool update_parax_rays)
{
    if(update_parax_rays) {
        compute_paraxial_rays();
    }

    int img = opt_sys_->optical_assembly()->image_index();
    int ref_wi = opt_sys_->optical_spec()->spectral_region()->reference_index();
    double ref_wvl = opt_sys_->optical_spec()->spectral_region()->reference_wvl();

    double n_0 = opt_sys_->optical_assembly()->gap(0)->material()->rindex(ref_wvl);
    double n_k = opt_sys_->optical_assembly()->image_space_gap()->material()->rindex(ref_wvl);

    ParaxialRay ax_ray = ax_rays_[ref_wi];
    ParaxialRay pr_ray = pr_rays_[ref_wi];
    
    /*
    ParaxialTrace *tracer = new ParaxialTrace(opt_sys_);
    ParaxialRay p_ray = tracer->trace_paraxial_ray_parallel_to_axis_at_s1();
    ParaxialRay q_ray = tracer->trace_paraxial_ray_with_slope_at_s1();
    delete tracer;
    */
   ParaxialRay p_ray = p_ray_;
   ParaxialRay q_ray = q_ray_;

    double ak1 = p_ray.at(img).ht;
    double bk1 = q_ray.at(img).ht;
    double ck1 = n_k*p_ray.at(img).slp;
    double dk1 = n_k*q_ray.at(img).slp;

    // fill in the content of first order data
    fod_.opt_inv = n_0 * ( ax_ray.at(1).ht*pr_ray.at(0).slp - pr_ray.at(1).ht*ax_ray.at(0).slp );
    fod_.obj_dist = opt_sys_->optical_assembly()->gap(0)->thi();
    fod_.img_dist = opt_sys_->optical_assembly()->image_space_gap()->thi();
    fod_.efl = -1.0/ck1;
    fod_.pp1 = (dk1 - 1.0)*(n_0/ck1);
    fod_.ppk = (p_ray.at(img-1).ht - 1.0)*(n_k/ck1);
    fod_.ffl = fod_.pp1 - fod_.efl;
    fod_.bfl = fod_.efl - fod_.ppk;
    fod_.fno = -1.0/(2.0*n_k*ax_ray.at(img).slp);

    fod_.m = ak1 + ck1*fod_.img_dist/n_k;
    fod_.red = dk1 + ck1*fod_.obj_dist;

    fod_.n_obj = n_0;
    fod_.n_img = n_k;

    fod_.img_ht = -fod_.opt_inv/(n_k*ax_ray.at(img).slp);
    fod_.obj_ang = atan(pr_ray.at(0).slp) * 180.0/M_PI;
    
    double nu_pr0 = n_0*pr_ray.at(0).slp;
    fod_.enp_dist = -pr_ray.at(1).ht/nu_pr0;
    fod_.enp_radius = abs(fod_.opt_inv/nu_pr0);

    fod_.exp_dist = -(pr_ray.at(img).ht/pr_ray.at(img).slp - fod_.img_dist);
    fod_.exp_radius = abs( fod_.opt_inv/(n_k*pr_ray.at(img).slp) );

    fod_.obj_na = n_0*sin( atan(ax_ray.at(0).slp) );
    fod_.img_na = n_k*sin( atan(ax_ray.at(img).slp) );
}

void ParaxialModel::get_starting_coords(double *y, double *u, double *ybar, double *ubar)
{
    int img_srf = opt_sys_->optical_assembly()->image_index();

    //int ref_wi = opt_sys_->optical_spec()->spectral_region()->reference_index();
    double ref_wvl = opt_sys_->optical_spec()->spectral_region()->reference_wvl();
    
    double n_0 = opt_sys_->optical_assembly()->gap(0)->material()->rindex(ref_wvl);
    double n_k = opt_sys_->optical_assembly()->image_space_gap()->material()->rindex(ref_wvl);

    /*
    auto p_ray = trace_paraxial_ray_parallel_to_axis_at_s1();
    auto q_ray = trace_paraxial_ray_with_slope_at_s1();
    */

    double ak1 = p_ray_.back().ht;
    double bk1 = q_ray_.back().ht;
    double ck1 = n_k*p_ray_.back().slp;
    double dk1 = n_k*q_ray_.back().slp;

    int stop = opt_sys_->optical_assembly()->stop_index();

    double n_s = opt_sys_->optical_assembly()->gap(stop)->material()->rindex(ref_wvl);
    double as1 = p_ray_.at(stop).ht;
    double bs1 = q_ray_.at(stop).ht;
    double cs1 = n_s*p_ray_.at(stop).slp;
    double ds1 = n_s*q_ray_.at(stop).slp;

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

    *y = y0;
    *u = u0;
    *ybar = ybar0;
    *ubar = ubar0;
}

Eigen::Matrix2d ParaxialModel::system_matrix(int start, int end, int wi) const
{
    assert(start > 0);
    assert(start < end);

    double wvl = opt_sys_->optical_spec()->spectral_region()->wavelength(wi);

    Eigen::Matrix2d system_mat = Eigen::Matrix2d::Identity(2,2);
    Eigen::Matrix2d refraction_mat;
    Eigen::Matrix2d transfer_mat;

    double n_before = opt_sys_->optical_assembly()->gap(start-1)->material()->rindex(wvl);
    
    for(int i = start; i <= end; i++) {
        double c       = opt_sys_->optical_assembly()->surface(i)->profile()->cv();
        double n_after = opt_sys_->optical_assembly()->gap(i)->material()->rindex(wvl);
        double phi     = c*(n_after - n_before);
        
        refraction_mat << 1, -phi, 0, 1;
        
        if(i == end){ // refraction at end surface
            system_mat = refraction_mat * system_mat;
        }else{ // refraction and transfer
            double d = opt_sys_->optical_assembly()->gap(i)->thi();
            transfer_mat << 1, 0, d/n_after, 1;

            system_mat = transfer_mat * (refraction_mat * system_mat);
        }

        n_before = n_after;
    }

    return system_mat;
}

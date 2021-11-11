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
**             Date: November 10th, 2021
********************************************************************************/

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <iomanip>
#include "system/paraxial_data.h"
#include "paraxial/paraxial_trace.h"

using namespace geopter;

ParaxialData::ParaxialData()
{

}

ParaxialData::~ParaxialData()
{
    clear();
}

void ParaxialData::clear()
{
    p_ray_.reset();
    q_ray_.reset();

    for(auto &r : ax_rays_){
        r.reset();
    }
    for(auto &r : pr_rays_){
        r.reset();
    }

    ax_rays_.clear();
    pr_rays_.clear();
}


void ParaxialData::update(OpticalSystem *opt_sys)
{
    this->clear();

    ParaxialTrace* tracer = new ParaxialTrace(opt_sys);

    // p/q rays
    p_ray_ = tracer->trace_paraxial_ray_parallel_to_axis_at_s1();
    q_ray_ = tracer->trace_paraxial_ray_with_slope_at_s1();

    // starting coords are computed with reference wavelength and are common for all wavelength
    double y0, u0, ybar0, ubar0;
    tracer->get_starting_coords(&y0, &u0, &ybar0, &ubar0);

    int img = opt_sys->optical_assembly()->image_index();

    const double ref_wvl_val = opt_sys->optical_spec()->spectral_region()->reference_wvl();
    const int ref_wvl_idx = opt_sys->optical_spec()->spectral_region()->reference_index();
    const int num_wvls = opt_sys->optical_spec()->spectral_region()->wvl_count();

    ax_rays_.clear();
    pr_rays_.clear();

    for(int wi = 0; wi < num_wvls; wi++)
    {
        double wvl = opt_sys->optical_spec()->spectral_region()->wvl(wi)->value();
        auto ax_ray_for_wvl = tracer->trace_paraxial_ray_from_object(y0, u0, wvl);
        ax_ray_for_wvl->set_name("axial ray traced with W" + std::to_string(wi));

        auto pr_ray_for_wvl = tracer->trace_paraxial_ray_from_object(ybar0, ubar0, wvl);
        pr_ray_for_wvl->set_name("principle ray traced with W" + std::to_string(wi));

        ax_rays_.push_back(ax_ray_for_wvl);
        pr_rays_.push_back(pr_ray_for_wvl);
    }

    delete tracer;

    auto ax_ray = ax_rays_[ref_wvl_idx];
    auto pr_ray = pr_rays_[ref_wvl_idx];

    auto p_ray = p_ray_;
    auto q_ray = q_ray_;

    double n_0 = opt_sys->optical_assembly()->gap(0)->material()->rindex(ref_wvl_val);
    double n_k = opt_sys->optical_assembly()->image_space_gap()->material()->rindex(ref_wvl_val);

    //double ak1 = p_ray->at(img)->y();
    //double bk1 = q_ray->at(img)->y();
    double ck1 = n_k*p_ray->at(img)->u_prime();
    double dk1 = n_k*q_ray->at(img)->u_prime();

    double y1 = ax_ray->at(1)->y();
    double ubar0_prime = pr_ray->at(0)->u_prime();
    double ybar1 = pr_ray->at(1)->y();
    double u0_prime = ax_ray->at(0)->u_prime();

    opt_inv_ = n_0 * ( y1*ubar0_prime - ybar1*u0_prime );
    efl_ = -1.0/ck1;
    fno_ = -1.0/(2.0*n_k*ax_ray->at(img)->u_prime());

    obj_dist_ = opt_sys->optical_assembly()->gap(0)->thi();
    img_dist_ = opt_sys->optical_assembly()->image_space_gap()->thi();

    red_ = dk1 + ck1*obj_dist_;

    pp1_ = (dk1 - 1.0)*(n_0/ck1);
    ppk_ = (p_ray->at(img-1)->y() - 1.0)*(n_k/ck1);
    ffl_ = pp1_ - efl_;
    bfl_ = efl_ - ppk_;

    n_obj_ = n_0;
    n_img_ = n_k;

    img_ht_ = -opt_inv_/(n_k*ax_ray->at(img)->u_prime());
    obj_ang_ = atan(pr_ray->at(0)->u_prime()) * 180.0/M_PI;

    double nu_pr0 = n_0*pr_ray->at(0)->u_prime();
    enp_dist_ = -ybar1/nu_pr0;
    enp_radius_ = fabs(opt_inv_/nu_pr0);
    if(std::isnan(enp_dist_)){
        enp_dist_ = 0.0;
    }
    if(std::isnan(enp_radius_)){
        enp_radius_ = 0.0;
    }

    exp_dist_ = -(pr_ray->at(img)->y()/pr_ray->at(img)->u_prime() - img_dist_);
    exp_radius_ = fabs( opt_inv_/(n_k*pr_ray->at(img)->u_prime()) );
    if(std::isnan(exp_dist_)){
        exp_dist_ = 0.0;
    }
    if(std::isnan(exp_radius_)){
        exp_radius_ = 0.0;
    }

    obj_na_ = n_0*sin( atan(ax_ray->at(0)->u_prime()) );
    img_na_ = n_k*sin( atan(ax_ray->at(img)->u_prime()) );

}

void ParaxialData::print(std::ostringstream& oss)
{
    constexpr int fixed_w = 30;
    constexpr int pre = 4;

    oss << std::setw(fixed_w) << std::left << "Effective Focal Length";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << efl_ << std::endl;

    oss << std::setw(fixed_w) << std::left << "Front Focal Length";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << ffl_ << std::endl;

    oss << std::setw(fixed_w) << std::left << "Back Focal Length";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << bfl_ << std::endl;

    oss << std::setw(fixed_w) << std::left << "F/#";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << fno_ << std::endl;

    oss << std::setw(fixed_w) << std::left << "Reduction Rate";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << red_ << std::endl;

    oss << std::setw(fixed_w) << std::left << "Object Distance";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << obj_dist_ << std::endl;

    oss << std::setw(fixed_w) << std::left << "Object Angle";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << obj_ang_ << std::endl;

    oss << std::setw(fixed_w) << std::left << "Image Distance";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << img_dist_ << std::endl;

    oss << std::setw(fixed_w) << std::left << "Image Height";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << img_ht_ << std::endl;

    oss << std::setw(fixed_w) << std::left << "Entrance Pupil Distance";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << enp_dist_ << std::endl;

    oss << std::setw(fixed_w) << std::left << "Entrance Pupil Radius";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << enp_radius_ << std::endl;

    oss << std::setw(fixed_w) << std::left << "Exit Pupil Distance";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << exp_dist_ << std::endl;

    oss << std::setw(fixed_w) << std::left << "Exit Pupil Radius";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << exp_radius_ << std::endl;

    oss << std::setw(fixed_w) << std::left << "Optical Invariant";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << opt_inv_ << std::endl;

    oss << std::endl;
}

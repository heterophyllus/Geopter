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

#include <iostream>

#include "paraxial/paraxial_trace.h"

using namespace geopter;

ParaxialTrace::ParaxialTrace(const OpticalSystem* sys)
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
    double n0_prime = n0;
    par_ray->append(y0, u0_prime, i0, n0_prime);

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

        par_ray->append(y, u_prime, i, n_prime);

        // transfer to next
        d = par_path.at(k).t;
        y = y + d*u_prime;
        u = u_prime;
        n = n_prime;
    }

    return par_ray;
}


std::shared_ptr<ParaxialRay> ParaxialTrace::trace_paraxial_axis_ray(double wvl)
{
    return trace_paraxial_ray_from_object(ref_y0_, ref_u0_, wvl);
}

std::shared_ptr<ParaxialRay> ParaxialTrace::trace_paraxial_chief_ray(double wvl)
{
    return trace_paraxial_ray_from_object(ref_ybar0_, ref_ubar0_, wvl);
}

ParaxialPath ParaxialTrace::paraxial_path(int start, int end, double wvl) const
{
    if(start <= end) {
        return forward_paraxial_path(start, end, wvl);
    } else {
        return reverse_paraxial_path(start, end, wvl);
    }
}

ParaxialPath ParaxialTrace::forward_paraxial_path(int start, int end, double wvl) const
{
    assert(start <= end);

    int num_gaps = opt_sys_->optical_assembly()->gap_count();

    ParaxialPathComponent par_path_comp;
    ParaxialPath par_path;

    for(int i = start; i <= end; i++) {
        par_path_comp.c = opt_sys_->optical_assembly()->surface(i)->cv();

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

ParaxialPath ParaxialTrace::reverse_paraxial_path(int start, int end, double wvl) const
{
    assert(start >= end);

    ParaxialPathComponent par_path_comp;
    ParaxialPath par_path;

    for(int i = start; i >= end; i--) {
        par_path_comp.c  = - opt_sys_->optical_assembly()->surface(i)->cv();

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
    double n_0     = opt_sys_->optical_assembly()->gap(0)->material()->rindex(ref_wvl);
    double n_k     = opt_sys_->optical_assembly()->image_space_gap()->material()->rindex(ref_wvl);
    int stop       = opt_sys_->optical_assembly()->stop_index();
    int last_surf  = opt_sys_->optical_assembly()->image_index() -1;

    Eigen::Vector2d y1_nu1;
    Eigen::Vector2d yk_nuk;
    Eigen::Matrix2d ABCD = this->system_matrix(1,last_surf, ref_wvl);


    // parallel to axis
    y1_nu1(0) = 1.0;
    y1_nu1(1) = 0.0;
    yk_nuk = ABCD*y1_nu1;
    double ck1 = yk_nuk(1);


    y1_nu1(0) = 0.0;
    y1_nu1(1) = 1/n_0;
    yk_nuk = ABCD*y1_nu1;
    double dk1 = yk_nuk(1);

    /*
    auto p_ray = trace_paraxial_ray_parallel_to_axis_at_s1();
    auto q_ray = trace_paraxial_ray_with_slope_at_s1();

    //double ak1 = p_ray->back()->y();
    //double bk1 = q_ray->back()->y();
    double ck1 = n_k*p_ray->back()->u_prime();
    double dk1 = n_k*q_ray->back()->u_prime();
    */

    Eigen::Matrix2d ABCDs = this->system_matrix(1,stop, ref_wvl);
    Eigen::Vector2d ys_nus;
    y1_nu1(0) = 1.0;
    y1_nu1(1) = 0.0;
    ys_nus = ABCDs*y1_nu1;
    double as1 = ys_nus(0);

    y1_nu1(0) = 0.0;
    y1_nu1(1) = 1/n_0;
    ys_nus = ABCD*y1_nu1;
    double bs1 = ys_nus(0);

    /*
    //double n_s = opt_sys_->optical_assembly()->gap(stop)->material()->rindex(ref_wvl);
    double as1 = p_ray->at(stop)->y();
    double bs1 = q_ray->at(stop)->y();
    //double cs1 = n_s*p_ray->at(stop)->u_prime();
    //double ds1 = n_s*q_ray->at(stop)->u_prime();

    */

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

Eigen::Matrix2d ParaxialTrace::system_matrix(int s1, int s2, double wvl) const
{
    /*
     *  |y'| =  |A B|  |y|
     *  |nu'|   |C D|  |nu|
     *
     */

    Eigen::Matrix2d M = Eigen::Matrix2d::Identity(2,2); // system matrix
    Eigen::Matrix2d T = Eigen::Matrix2d::Identity(2,2); // transfer
    Eigen::Matrix2d R = Eigen::Matrix2d::Identity(2,2); // refract

    ParaxialPath path = paraxial_path(s1, s2, wvl);

    double n, n_prime;

    if( s1 > 0){
        n = opt_sys_->optical_assembly()->gap(s1-1)->material()->rindex(wvl);
    }else{
        n = opt_sys_->optical_assembly()->gap(0)->material()->rindex(wvl);
    }

    for(int i = 0; i < path.size()-1; i++) {
        n_prime = path.at(i).n;

        // refract
        double phi = (n_prime - n) * path.at(i).c;
        R(1,0) = -phi;
        M = R*M;

        // transfer
        T(0,1) = path.at(i).t/n_prime;
        M = T*M;

        n = n_prime;
    }

    // refract at s2
    n_prime = path.back().n;
    R(1,0) = -( (n_prime - n) * path.back().c );
    M = R*M;

    return M;
}

void ParaxialTrace::compute_first_order_data(FirstOrderData* fod, double wvl)
{

    // starting coords are computed with reference wavelength and are common for all wavelength
    double y0, u0, ybar0, ubar0;
    this->get_starting_coords(&y0, &u0, &ybar0, &ubar0);

    const int img = opt_sys_->optical_assembly()->image_index();
    const int last_surf = opt_sys_->optical_assembly()->image_index() -1;
    const int stop = opt_sys_->optical_assembly()->stop_index();
    const double ref_wvl_val = opt_sys_->optical_spec()->spectral_region()->reference_wvl();

    const double n_0 = opt_sys_->optical_assembly()->gap(0)->material()->rindex(ref_wvl_val);
    const double n_k = opt_sys_->optical_assembly()->image_space_gap()->material()->rindex(ref_wvl_val);

    auto ax_ray = trace_paraxial_ray_from_object(y0, u0, wvl);
    auto pr_ray = trace_paraxial_ray_from_object(ybar0, ubar0, wvl);

    Eigen::Vector2d y_nu1_p(1.0, 0.0);
    Eigen::Vector2d y_nu1_q(0.0, 1.0/n_0);
    Eigen::Vector2d y_nuk_p;
    Eigen::Vector2d y_nuk_q;
    Eigen::Matrix2d Mk = this->system_matrix(1,last_surf, wvl);
    Eigen::Matrix2d Ms = system_matrix(1, stop, wvl);

    y_nuk_p = Mk*y_nu1_p;
    y_nuk_q = Mk*y_nu1_q;

    double ck1 = y_nuk_p(1);
    double dk1 = y_nuk_q(1);

    /*
    //double ak1 = p_ray->at(img)->y();
    //double bk1 = q_ray->at(img)->y();
    double ck1 = n_k*p_ray->at(img)->u_prime();
    double dk1 = n_k*q_ray->at(img)->u_prime();
    */

    double y1 = ax_ray->at(1)->y();
    double ubar0_prime = pr_ray->at(0)->u_prime();
    double ybar1 = pr_ray->at(1)->y();
    double u0_prime = ax_ray->at(0)->u_prime();

    fod->opt_inv = n_0 * ( y1*ubar0_prime - ybar1*u0_prime );
    fod->efl = -1.0/ck1;
    fod->fno = -1.0/(2.0*n_k*ax_ray->at(img)->u_prime());

    fod->obj_dist = opt_sys_->optical_assembly()->gap(0)->thi();
    fod->img_dist = opt_sys_->optical_assembly()->image_space_gap()->thi();

    fod->red = dk1 + ck1*fod->obj_dist;

    fod->pp1 = (dk1 - 1.0)*(n_0/ck1);
    //fod->ppk = (p_ray->at(img-1)->y() - 1.0)*(n_k/ck1);
    fod->ppk = (y_nuk_p(0)-1.0)*(n_k/ck1);
    fod->ffl = fod->pp1 - fod->efl;
    fod->bfl = fod->efl - fod->ppk;

    fod->n_obj = n_0;
    fod->n_img = n_k;

    fod->img_ht = -fod->opt_inv/(n_k*ax_ray->at(img)->u_prime());
    fod->obj_ang = atan(pr_ray->at(0)->u_prime()) * 180.0/M_PI;

    double nu_pr0 = n_0*pr_ray->at(0)->u_prime();
    fod->enp_dist = -ybar1/nu_pr0;
    fod->enp_radius = fabs(fod->opt_inv/nu_pr0);
    if(std::isnan(fod->enp_dist)){
        fod->enp_dist = 1.0e+10;
    }
    if(std::isnan(fod->enp_radius)){
        fod->enp_radius = 1.0e+10;
    }

    fod->exp_dist = -(pr_ray->at(img)->y()/pr_ray->at(img)->u_prime() - fod->img_dist);
    fod->exp_radius = fabs( fod->opt_inv/(n_k*pr_ray->at(img)->u_prime()) );
    if(std::isnan(fod->exp_dist)){
        fod->exp_dist = -1.0e+10;
    }
    if(std::isnan(fod->exp_radius)){
        fod->exp_radius = -1.0e+10;
    }

    fod->obj_na = n_0*sin( atan(ax_ray->at(0)->u_prime()) );
    fod->img_na = n_k*sin( atan(ax_ray->at(img)->u_prime()) );

}


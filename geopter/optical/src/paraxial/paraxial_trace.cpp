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

#include "paraxial/paraxial_trace.h"

#include <iostream>

using namespace geopter;

ParaxialTrace::ParaxialTrace(const OpticalSystem* sys)
{
    opt_sys_ = sys;
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
    double y0 = opt_sys_->first_order_data()->ref_u0;
    double u0 = opt_sys_->first_order_data()->ref_u0;

    return trace_paraxial_ray_from_object(y0, u0, wvl);
}

std::shared_ptr<ParaxialRay> ParaxialTrace::trace_paraxial_chief_ray(double wvl)
{
    double ybar0 = opt_sys_->first_order_data()->ref_ybar0;
    double ubar0 = opt_sys_->first_order_data()->ref_ubar0;

    return trace_paraxial_ray_from_object(ybar0, ubar0, wvl);
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

Eigen::Matrix2d ParaxialTrace::system_matrix(OpticalSystem* opt_sys, int s1, int s2, double wvl)
{
    /*
     *  |y'| =  |A B|  |y|
     *  |nu'|   |C D|  |nu|
     *
     */

    Eigen::Matrix2d M = Eigen::Matrix2d::Identity(2,2); // system matrix
    Eigen::Matrix2d T = Eigen::Matrix2d::Identity(2,2); // transfer
    Eigen::Matrix2d R = Eigen::Matrix2d::Identity(2,2); // refract

    double n, n_prime;

    if( s1 > 0){
        n = opt_sys->optical_assembly()->gap(s1-1)->material()->rindex(wvl);
    }else{
        n = opt_sys->optical_assembly()->gap(0)->material()->rindex(wvl);
    }

    for(int i = s1; i < s2; i++) {
        n_prime = opt_sys->optical_assembly()->gap(i)->material()->rindex(wvl);

        // refract
        double c = opt_sys->optical_assembly()->surface(i)->cv();
        double phi = (n_prime - n) * c;
        R(1,0) = -phi;
        M = R*M;

        // transfer
        double t = opt_sys->optical_assembly()->gap(i)->thi();
        T(0,1) = t/n_prime;
        M = T*M;

        n = n_prime;
    }

    // refract at s2
    n_prime = opt_sys->optical_assembly()->gap(s2)->material()->rindex(wvl);
    double c_s2 = opt_sys->optical_assembly()->surface(s2)->cv();
    R(1,0) = -( (n_prime - n) * c_s2 );
    M = R*M;

    return M;
}





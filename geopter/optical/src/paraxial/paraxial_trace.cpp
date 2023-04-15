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

std::shared_ptr<ParaxialRay> ParaxialTrace::TraceParaxialRayFromObject(double y0, double u0, double wvl)
{
    auto par_ray = std::make_shared<ParaxialRay>();
    int img = opt_sys_->GetOpticalAssembly()->ImageIndex();

    ParaxialPath par_path = CreateParaxialPath(0, img, wvl);
    int path_size = par_path.Size();

    if(path_size == 0) {
        return par_ray; // empty
    }

    // object surface
    double c0 = par_path.At(0).curvature;
    double u0_prime = u0;
    double i0 = y0*c0 + u0;
    double n0 = par_path.At(0).refractive_index;
    double n0_prime = n0;
    par_ray->Append(y0, u0_prime, i0, n0_prime);

    if(path_size == 1) {
        return par_ray;
    }


    // trace the rest
    double d = par_path.At(0).thickness;
    double y = y0 + d*u0_prime;
    double u = u0_prime;
    double c;
    double i, i_prime;
    double n = n0;
    double n_prime;
    double u_prime;
    
    for(int k = 1; k < path_size; k++) {
        c = par_path.At(k).curvature;
        n_prime = par_path.At(k).refractive_index;
        i = u + y*c;
        i_prime = i*(n/n_prime);
        u_prime = i_prime - y*c;

        par_ray->Append(y, u_prime, i, n_prime);

        // transfer to next
        d = par_path.At(k).thickness;
        y = y + d*u_prime;
        u = u_prime;
        n = n_prime;
    }

    return par_ray;
}


std::shared_ptr<ParaxialRay> ParaxialTrace::TraceParaxialAxisRay(double wvl)
{
    double y0 = opt_sys_->GetFirstOrderData()->reference_u0;
    double u0 = opt_sys_->GetFirstOrderData()->reference_u0;

    return TraceParaxialRayFromObject(y0, u0, wvl);
}

std::shared_ptr<ParaxialRay> ParaxialTrace::TraceParaxialChiefRay(double wvl)
{
    double ybar0 = opt_sys_->GetFirstOrderData()->reference_ybar0;
    double ubar0 = opt_sys_->GetFirstOrderData()->reference_ubar0;

    return TraceParaxialRayFromObject(ybar0, ubar0, wvl);
}

ParaxialPath ParaxialTrace::CreateParaxialPath(int start, int end, double wvl) const
{
    if(start <= end) {
        return CreateForwardParaxialPath(start, end, wvl);
    } else {
        return CreateReverseParaxialPath(start, end, wvl);
    }
}

ParaxialPath ParaxialTrace::CreateForwardParaxialPath(int start, int end, double wvl) const
{
    assert(start <= end);

    int num_gaps = opt_sys_->GetOpticalAssembly()->NumberOfGaps();

    ParaxialPathComponent par_path_comp;
    ParaxialPath par_path;

    for(int i = start; i <= end; i++) {
        par_path_comp.curvature = opt_sys_->GetOpticalAssembly()->GetSurface(i)->Curvature();

        if( i < num_gaps ){
            par_path_comp.thickness = opt_sys_->GetOpticalAssembly()->GetGap(i)->Thickness();
            par_path_comp.refractive_index = opt_sys_->GetOpticalAssembly()->GetGap(i)->GetMaterial()->RefractiveIndex(wvl);
        }else{
            par_path_comp.thickness = 0.0;
            par_path_comp.refractive_index = 1.0;
        }

        par_path.Append(par_path_comp);
    }

    return par_path;
}

ParaxialPath ParaxialTrace::CreateReverseParaxialPath(int start, int end, double wvl) const
{
    assert(start >= end);

    ParaxialPathComponent par_path_comp;
    ParaxialPath par_path;

    for(int i = start; i >= end; i--) {
        par_path_comp.curvature  = - opt_sys_->GetOpticalAssembly()->GetSurface(i)->Curvature();

        if( i > 0 ){
            par_path_comp.thickness = opt_sys_->GetOpticalAssembly()->GetGap(i-1)->Thickness();
            par_path_comp.refractive_index = opt_sys_->GetOpticalAssembly()->GetGap(i-1)->GetMaterial()->RefractiveIndex(wvl);
        }else{
            par_path_comp.thickness = 0.0;
            par_path_comp.refractive_index = 1.0;
        }

        par_path.Append(par_path_comp);
    }

    return par_path;
}


Eigen::Matrix2d ParaxialTrace::SystemMatrix(int s1, int s2, double wvl) const
{
    /*
     *  |y'| =  |A B|  |y|
     *  |nu'|   |C D|  |nu|
     *
     */

    Eigen::Matrix2d M = Eigen::Matrix2d::Identity(2,2); // system matrix
    Eigen::Matrix2d T = Eigen::Matrix2d::Identity(2,2); // transfer
    Eigen::Matrix2d R = Eigen::Matrix2d::Identity(2,2); // refract

    ParaxialPath path = CreateParaxialPath(s1, s2, wvl);

    double n, n_prime;

    if( s1 > 0){
        n = opt_sys_->GetOpticalAssembly()->GetGap(s1-1)->GetMaterial()->RefractiveIndex(wvl);
    }else{
        n = opt_sys_->GetOpticalAssembly()->GetGap(0)->GetMaterial()->RefractiveIndex(wvl);
    }

    for(int i = 0; i < path.Size()-1; i++) {
        n_prime = path.At(i).refractive_index;

        // refract
        double phi = (n_prime - n) * path.At(i).curvature;
        R(1,0) = -phi;
        M = R*M;

        // transfer
        T(0,1) = path.At(i).thickness/n_prime;
        M = T*M;

        n = n_prime;
    }

    // refract at s2
    n_prime = path.Back().refractive_index;
    R(1,0) = -( (n_prime - n) * path.Back().curvature );
    M = R*M;

    return M;
}

Eigen::Matrix2d ParaxialTrace::SystemMatrix(OpticalSystem* opt_sys, int s1, int s2, double wvl)
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
        n = opt_sys->GetOpticalAssembly()->GetGap(s1-1)->GetMaterial()->RefractiveIndex(wvl);
    }else{
        n = opt_sys->GetOpticalAssembly()->GetGap(0)->GetMaterial()->RefractiveIndex(wvl);
    }

    for(int i = s1; i < s2; i++) {
        n_prime = opt_sys->GetOpticalAssembly()->GetGap(i)->GetMaterial()->RefractiveIndex(wvl);

        // refract
        double c = opt_sys->GetOpticalAssembly()->GetSurface(i)->Curvature();
        double phi = (n_prime - n) * c;
        R(1,0) = -phi;
        M = R*M;

        // transfer
        double t = opt_sys->GetOpticalAssembly()->GetGap(i)->Thickness();
        T(0,1) = t/n_prime;
        M = T*M;

        n = n_prime;
    }

    // refract at s2
    n_prime = opt_sys->GetOpticalAssembly()->GetGap(s2)->GetMaterial()->RefractiveIndex(wvl);
    double c_s2 = opt_sys->GetOpticalAssembly()->GetSurface(s2)->Curvature();
    R(1,0) = -( (n_prime - n) * c_s2 );
    M = R*M;

    return M;
}





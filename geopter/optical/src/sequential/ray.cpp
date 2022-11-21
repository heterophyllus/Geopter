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

#include "sequential/ray.h"

#include <iostream>
#include <iomanip>


using namespace geopter;

Ray::Ray() :
    status_(TRACE_NOT_REACHED_ERROR),
    wvl_(0.0),
    size_(0),
    reached_surface_index_(0)
{
    pupil_crd_ = Eigen::Vector2d::Zero(2);
}

Ray::Ray(int n) :
    status_(TRACE_SUCCESS),
    wvl_(0.0),
    reached_surface_index_(0)
{
    this->allocate(n);
}

Ray::~Ray()
{
    for(auto &r : ray_at_srfs_){
        r.reset();
    }
    ray_at_srfs_.clear();
}

void Ray::allocate(int n)
{
    this->clear();

    ray_at_srfs_.reserve(n);
    RaySegment* before = nullptr;
    for(int i = 0; i < n; i++){
        ray_at_srfs_.emplace_back( std::make_unique<RaySegment>() );
        ray_at_srfs_.back()->set_before(before);
        before = ray_at_srfs_.back().get();
    }
    size_ = ray_at_srfs_.size();
}

void Ray::prepend(std::unique_ptr<RaySegment> ray_at_srf)
{
    ray_at_srfs_.insert(ray_at_srfs_.begin(), std::move(ray_at_srf));
    ray_at_srfs_.front()->set_before( ray_at_srfs_[1].get() );
    size_ += 1;
}


void Ray::append(const Eigen::Vector3d& inc_pt, const Eigen::Vector3d& normal, const Eigen::Vector3d& after_dir, double dist, double opl)
{
    RaySegment *before;
    if(ray_at_srfs_.empty()){
        before = nullptr;
    }else{
        before = ray_at_srfs_.back().get();
    }

    int i = size_-1;
    ray_at_srfs_.emplace_back(std::make_unique<RaySegment>(i,inc_pt, normal, after_dir, dist, opl, before));
    opl_ += opl;
    size_ += 1;
}


void Ray::clear()
{
    if( !ray_at_srfs_.empty() ){
        for(auto &r : ray_at_srfs_){
            r.reset();
        }
        ray_at_srfs_.clear();
    }
    size_ = 0;
}

void Ray::set_reached_surface(int i)
{
    reached_surface_index_ = i;

    /*
    if( i < (int)ray_at_srfs_.size()-1 ){
        for(int j = i+1; j < (int)ray_at_srfs_.size(); j++){
            ray_at_srfs_[j] = nullptr;
        }
    }
    */

}

double Ray::optical_path_length() const
{
    double opl_tot = 0.0;
    int last = ray_at_srfs_.size()-1;
    for(int i = 2; i < last; i++){
        opl_tot += ray_at_srfs_[i]->optical_path_length();
    }
    return opl_tot;
}

void Ray::print(std::ostringstream& oss)
{
    const int idx_w = 4;
    const int val_w = 10;
    const int prec  = 4;

    int num_srfs = ray_at_srfs_.size();

    //status
    oss << "Status: ";
    switch (status_) {
    case TRACE_SUCCESS:
        oss << "Pass Through";
        break;
    case TRACE_BLOCKED_ERROR:
        oss << "Blocked";
        break;
    case TRACE_MISSEDSURFACE_ERROR:
        oss << "Missed Surface";
        break;
    case TRACE_TIR_ERROR                                                                                                                                                                                                                                                                                 :
        oss << "Total Reflection";
        break;
    default:
        oss << "Unknown Status";
    }
    oss << std::endl;


    // wavelength
    oss << "Wavelength: " << wvl_ << "nm" << std::endl;

    oss << std::endl;

    // intercept
    // headder label, S   X   Y   Z   AOI
    oss << std::setw(idx_w) << std::right << "S";
    oss << std::setw(val_w) << std::right << "X";
    oss << std::setw(val_w) << std::right << "Y";
    oss << std::setw(val_w) << std::right << "Z";
    oss << std::setw(val_w) << std::right << "L";
    oss << std::setw(val_w) << std::right << "M";
    oss << std::setw(val_w) << std::right << "N";
    oss << std::setw(val_w) << std::right << "AOI";
    oss << std::endl;

    for(int si = 0; si < num_srfs; si++)
    {
        Eigen::Vector3d intercept = ray_at_srfs_[si]->intersect_pt();
        oss << std::setw(idx_w) << std::right << si;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << intercept(0);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << intercept(1);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << intercept(2);

        Eigen::Vector3d after_dir = ray_at_srfs_[si]->after_dir();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << after_dir(0);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << after_dir(1);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << after_dir(2);

        double aoi = ray_at_srfs_[si]->aoi();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << aoi;

        oss << std::endl;
    }

    oss << std::endl;
}

void Ray::print()
{
    std::ostringstream oss;
    this->print(oss);
    std::cout << oss.str() << std::endl;
}

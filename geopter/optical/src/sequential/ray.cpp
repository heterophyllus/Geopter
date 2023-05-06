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

#include "sequential/ray.h"

#include <iostream>
#include <iomanip>


using namespace geopter;

Ray::Ray() :
    status_(TRACE_NOT_REACHED_ERROR),
    wvl_(0.0),
    num_segments_(0),
    reached_surface_index_(0)
{
    pupil_crd_ = Eigen::Vector2d::Zero(2);
}

Ray::Ray(int n) :
    status_(TRACE_NOT_REACHED_ERROR),
    wvl_(0.0),
    num_segments_(0),
    reached_surface_index_(0)
{
    pupil_crd_ = Eigen::Vector2d::Zero(2);
    this->Allocate(n);
}



Ray::~Ray()
{
    for(auto &r : segments_){
        r.reset();
    }
    segments_.clear();
}

void Ray::Allocate(int n)
{
    this->Clear();

    segments_.reserve(n);
    RaySegment* before = nullptr;
    for(int i = 0; i < n; i++){
        segments_.emplace_back( std::make_unique<RaySegment>() );
        segments_.back()->SetBefore(before);
        before = segments_.back().get();
    }
    num_segments_ = segments_.size();
}

void Ray::Prepend(std::unique_ptr<RaySegment> ray_at_srf)
{
    segments_.insert(segments_.begin(), std::move(ray_at_srf));
    segments_.front()->SetBefore( segments_[1].get() );
    num_segments_ += 1;
}


void Ray::Append(const Eigen::Vector3d& inc_pt, const Eigen::Vector3d& normal, const Eigen::Vector3d& after_dir, double dist, double opl)
{
    RaySegment *before;
    if(segments_.empty()){
        before = nullptr;
    }else{
        before = segments_.back().get();
    }

    int i = num_segments_-1;
    segments_.emplace_back(std::make_unique<RaySegment>(i,inc_pt, normal, after_dir, dist, opl, before));
    opl_ += opl;
    num_segments_ += 1;
}


void Ray::Clear()
{
    if( !segments_.empty() ){
        for(auto &r : segments_){
            r.reset();
        }
        segments_.clear();
    }
    num_segments_ = 0;
}

void Ray::SetReachedSurfaceIndex(int i)
{
    reached_surface_index_ = i;
}

double Ray::OpticalPathLength() const
{
    double opl_tot = 0.0;
    int last = segments_.size()-1;
    for(int i = 2; i < last; i++){
        opl_tot += segments_[i]->OpticalPathLength();
    }
    return opl_tot;
}

void Ray::Print(std::ostringstream& oss)
{
    const int idx_w = 4;
    const int val_w = 10;
    const int prec  = 4;

    int num_srfs = segments_.size();

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
        Eigen::Vector3d intercept = segments_[si]->IntersectPt();
        oss << std::setw(idx_w) << std::right << si;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << intercept(0);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << intercept(1);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << intercept(2);

        Eigen::Vector3d after_dir = segments_[si]->Direction();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << after_dir(0);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << after_dir(1);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << after_dir(2);

        double aoi = segments_[si]->AngleOfIncidence();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << aoi;

        oss << std::endl;
    }

    oss << std::endl;
}

void Ray::Print()
{
    std::ostringstream oss;
    this->Print(oss);
    std::cout << oss.str() << std::endl;
}

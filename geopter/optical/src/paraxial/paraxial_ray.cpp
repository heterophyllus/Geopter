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

#include "paraxial/paraxial_ray.h"

#include <iostream>
#include <iomanip>

using namespace geopter;

ParaxialRay::ParaxialRay() :
    name_("")
{

}

ParaxialRay::ParaxialRay(int n) :
    name_("")
{
    prx_ray_segments_.resize(n);
}

ParaxialRay::~ParaxialRay()
{
    prx_ray_segments_.clear();
}

void ParaxialRay::clear()
{
    prx_ray_segments_.clear();
    name_ = "";
}


void ParaxialRay::prepend(const ParaxialRaySegment& segment)
{
    auto itr = prx_ray_segments_.begin();
    prx_ray_segments_.insert(itr, segment);
}

void ParaxialRay::prepend(double y, double u_prime, double i, double n_prime)
{
    auto prx_ray_at_srf = ParaxialRaySegment(y, u_prime, i, n_prime);
    auto itr = prx_ray_segments_.begin();
    prx_ray_segments_.insert( itr, prx_ray_at_srf);
}

void ParaxialRay::append(const ParaxialRaySegment& segment)
{
    prx_ray_segments_.push_back(segment);
}

void ParaxialRay::append(double y, double u_prime, double i, double n_prime)
{
    prx_ray_segments_.emplace_back(ParaxialRaySegment(y, u_prime, i, n_prime));
}


void ParaxialRay::print() const
{
    std::ostringstream oss;
    this->print(oss);
    std::cout << oss.str() << std::endl;
}

void ParaxialRay::print(std::ostringstream& oss) const
{
    constexpr int idx_w  = 4;
    constexpr int val_w  = 10;
    constexpr int prec   = 4;

    int num_srf = prx_ray_segments_.size();

    oss << std::left << name_ << std::endl;

    oss << std::setw(idx_w) << std::right << "S";
    oss << std::setw(val_w) << std::right << "h";
    oss << std::setw(val_w) << std::right << "n\'";
    oss << std::setw(val_w) << std::right << "u\'";
    oss << std::setw(val_w) << std::right << "(nu)\'";
    oss << std::setw(val_w) << std::right << "i";
    oss << std::endl;

    for(int i = 0; i < num_srf; i++)
    {
        oss << std::setw(idx_w) << std::right << i;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << prx_ray_segments_[i].y();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << prx_ray_segments_[i].n_prime();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << prx_ray_segments_[i].u_prime();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << prx_ray_segments_[i].n_prime()*prx_ray_segments_[i].u_prime();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << prx_ray_segments_[i].i();
        oss << std::endl;
    }
    oss << std::endl;
}

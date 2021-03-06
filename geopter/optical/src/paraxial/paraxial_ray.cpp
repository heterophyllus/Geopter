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

#include <iostream>
#include <iomanip>

#include "paraxial/paraxial_ray.h"

using namespace geopter;

ParaxialRay::ParaxialRay() :
    name_("")
{

}

ParaxialRay::~ParaxialRay()
{
    par_ray_at_srfs_.clear();
}

void ParaxialRay::clear()
{
    par_ray_at_srfs_.clear();
    name_ = "";
}

int ParaxialRay::size() const
{
    return (int)par_ray_at_srfs_.size();
}

void ParaxialRay::prepend(std::shared_ptr<ParaxialRayAtSurface> par_ray_at_srf)
{
    auto itr = par_ray_at_srfs_.begin();
    par_ray_at_srfs_.insert(itr, par_ray_at_srf);
}

void ParaxialRay::prepend(double y, double u_prime, double i, double n_prime)
{
    auto prx_ray_at_srf = std::make_shared<ParaxialRayAtSurface>(y, u_prime, i, n_prime);
    auto itr = par_ray_at_srfs_.begin();
    par_ray_at_srfs_.insert( itr, prx_ray_at_srf);
}

void ParaxialRay::append(std::shared_ptr<ParaxialRayAtSurface> par_ray_at_srf)
{
    ParaxialRayAtSurface *before = par_ray_at_srfs_.back().get();
    par_ray_at_srf->set_before(before);
    par_ray_at_srfs_.push_back(par_ray_at_srf);
}

void ParaxialRay::append(double y, double u_prime, double i, double n_prime)
{
    auto prx_ray_at_srf = std::make_shared<ParaxialRayAtSurface>(y, u_prime, i, n_prime);

    if(par_ray_at_srfs_.size() > 0){
        ParaxialRayAtSurface *before = par_ray_at_srfs_.back().get();
        prx_ray_at_srf->set_before(before);
    }

    par_ray_at_srfs_.push_back(prx_ray_at_srf);
}

std::string ParaxialRay::name() const
{
    return name_;
}

void ParaxialRay::set_name(std::string name)
{
    name_ = name;
}

std::shared_ptr<ParaxialRayAtSurface> ParaxialRay::at(int i) const
{
    return par_ray_at_srfs_[i];
}

std::shared_ptr<ParaxialRayAtSurface> ParaxialRay::front() const
{
    return par_ray_at_srfs_.front();
}

std::shared_ptr<ParaxialRayAtSurface> ParaxialRay::back() const
{
    return par_ray_at_srfs_.back();
}

void ParaxialRay::print() const
{
    std::ostringstream oss;
    this->print(oss);
    std::cout << oss.str() << std::endl;
}

std::shared_ptr<ParaxialRayAtSurface> ParaxialRay::at_before_image() const
{
    int img_idx = par_ray_at_srfs_.size() - 1;
    return par_ray_at_srfs_[img_idx - 1];
}

void ParaxialRay::print(std::ostringstream& oss) const
{
    constexpr int idx_w  = 4;
    constexpr int val_w  = 10;
    constexpr int prec   = 4;

    int num_srf = par_ray_at_srfs_.size();

    oss << std::left << name_ << std::endl;

    oss << std::setw(idx_w) << std::right << "S";
    oss << std::setw(val_w) << std::right << "h";
    oss << std::setw(val_w) << std::right << "u\'";
    oss << std::setw(val_w) << std::right << "(nu)\'";
    oss << std::setw(val_w) << std::right << "i";
    oss << std::setw(val_w) << std::right << "ni";
    oss << std::endl;

    for(int i = 0; i < num_srf; i++)
    {
        oss << std::setw(idx_w) << std::right << i;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << par_ray_at_srfs_[i]->y();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << par_ray_at_srfs_[i]->u_prime();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << par_ray_at_srfs_[i]->n_prime()*par_ray_at_srfs_[i]->u_prime();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << par_ray_at_srfs_[i]->i();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << par_ray_at_srfs_[i]->n() * par_ray_at_srfs_[i]->i();
        oss << std::endl;
    }
    oss << std::endl;
}

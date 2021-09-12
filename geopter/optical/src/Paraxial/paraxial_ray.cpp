#include <iostream>
#include <iomanip>

#include "Paraxial/paraxial_ray.h"

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

void ParaxialRay::prepend(ParaxialRayAtSurface par_ray_at_srf)
{
    auto itr = par_ray_at_srfs_.begin();
    par_ray_at_srfs_.insert(itr, par_ray_at_srf);
}

void ParaxialRay::prepend(double ht, double slp, double aoi, double n)
{
    auto itr = par_ray_at_srfs_.begin();
    par_ray_at_srfs_.insert( itr, ParaxialRayAtSurface(ht,slp,aoi,n) );
}

void ParaxialRay::append(ParaxialRayAtSurface par_ray_at_srf)
{
    par_ray_at_srfs_.push_back(par_ray_at_srf);
}

void ParaxialRay::append(double ht, double slp, double aoi, double n)
{
    ParaxialRayAtSurface par_ray_at_srf(ht, slp, aoi, n);
    par_ray_at_srfs_.push_back(par_ray_at_srf);
}

std::string ParaxialRay::name() const
{
    return name_;
}

void ParaxialRay::set_name(std::string name)
{
    name_ = name;
}

ParaxialRayAtSurface ParaxialRay::at(int i) const
{
    return par_ray_at_srfs_[i];
}

ParaxialRayAtSurface ParaxialRay::front() const
{
    return par_ray_at_srfs_.front();
}

ParaxialRayAtSurface ParaxialRay::back() const
{
    return par_ray_at_srfs_.back();
}

void ParaxialRay::print() const
{
    std::ostringstream oss;
    this->print(oss);
    std::cout << oss.str() << std::endl;
}

ParaxialRayAtSurface ParaxialRay::at_before_image() const
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
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << par_ray_at_srfs_[i].ht;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << par_ray_at_srfs_[i].slp;
        if( i == num_srf-1){
            oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << par_ray_at_srfs_[i].slp;
        }else{
            oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << par_ray_at_srfs_[i+1].n*par_ray_at_srfs_[i].slp;
        }
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << par_ray_at_srfs_[i].aoi;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << par_ray_at_srfs_[i].n * par_ray_at_srfs_[i].aoi;
        oss << std::endl;
    }
    oss << std::endl;
}

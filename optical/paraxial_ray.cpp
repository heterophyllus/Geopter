#include <sstream>
#include <iomanip>

#include "paraxial_ray.h"



using namespace geopter;

ParaxialRay::ParaxialRay()
{

}

ParaxialRay::~ParaxialRay()
{
    par_ray_at_srfs_.clear();
}

void ParaxialRay::clear()
{
    par_ray_at_srfs_.clear();
}

int ParaxialRay::size() const
{
    return (int)par_ray_at_srfs_.size();
}

void ParaxialRay::append(ParaxialRayAtSurface par_comp)
{
    par_ray_at_srfs_.push_back(par_comp);
}

void ParaxialRay::append(double ht, double slp, double aoi, double n)
{
    ParaxialRayAtSurface par_comp;
    par_comp.ht  = ht;
    par_comp.slp = slp;
    par_comp.aoi = aoi;
    par_comp.n   = n;

    par_ray_at_srfs_.push_back(par_comp);
}

ParaxialRayAtSurface ParaxialRay::at(int i)
{
    if( i < 0 ) {
        return par_ray_at_srfs_[par_ray_at_srfs_.size() + i];
    }else if( i < (int)par_ray_at_srfs_.size()) {
        return par_ray_at_srfs_[i];
    }else {
        ParaxialRayAtSurface tmp;
        return tmp;
    }
}


ParaxialRayAtSurface ParaxialRay::at_first()
{
    return par_ray_at_srfs_[1];
}


ParaxialRayAtSurface ParaxialRay::at_last()
{
    int image_idx = par_ray_at_srfs_.size() -1;
    return par_ray_at_srfs_[image_idx -1];
}

ParaxialRayAtSurface ParaxialRay::at_image()
{
    int image_idx = par_ray_at_srfs_.size() -1;
    return par_ray_at_srfs_[image_idx];
}

void ParaxialRay::print() const
{
    std::ostringstream oss;
    this->print(oss);
    std::cout << oss.str() << std::endl;
}

void ParaxialRay::print(std::ostringstream& oss) const
{
    const int idx_w  = 4;
    const int val_w  = 10;
    const int prec   = 4;

    int num_srf = par_ray_at_srfs_.size();

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
    oss << std::ends;
}

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

int ParaxialRay::size()
{
    return (int)par_ray_at_srfs_.size();
}

void ParaxialRay::append(ParaxialRayAtSurface par_comp)
{
    par_ray_at_srfs_.push_back(par_comp);
}

void ParaxialRay::append(double ht, double slp, double aoi)
{
    ParaxialRayAtSurface par_comp;
    par_comp.ht = ht;
    par_comp.slp = slp;
    par_comp.aoi = aoi;

    par_ray_at_srfs_.push_back(par_comp);
}

ParaxialRayAtSurface ParaxialRay::at(int i)
{
    return par_ray_at_srfs_[i];
}

ParaxialRayAtSurface ParaxialRay::back()
{
    return par_ray_at_srfs_.back();
}

void ParaxialRay::print()
{
    std::ostringstream oss;
    this->print(oss);
    std::cout << oss.str() << std::endl;
}

void ParaxialRay::print(std::ostringstream& oss)
{
    const int idx_w  = 4;
    const int val_w  = 10;
    const int prec   = 4;

    int num_srf = par_ray_at_srfs_.size();

    oss << std::setw(idx_w) << std::right << "S";
    oss << std::setw(val_w) << std::right << "HT";
    oss << std::setw(val_w) << std::right << "SLP";
    oss << std::endl;

    for(int i = 0; i < num_srf; i++)
    {
        oss << std::setw(idx_w) << std::right << i;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << par_ray_at_srfs_[i].ht;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << par_ray_at_srfs_[i].slp;
        oss << std::endl;
    }
    oss << std::ends;
}

#include <iostream>
#include <iomanip>
#include <sstream>

#include "ray.h"


using namespace geopter;

Ray::Ray()
{
    status_ = RayStatus::PassThrough;
}

Ray::~Ray()
{
    ray_at_srfs_.clear();
}

int Ray::size() const
{
    return (int)ray_at_srfs_.size();
}

void Ray::append(RayAtSurface ray_at_srf)
{
    ray_at_srfs_.push_back(ray_at_srf);
}

void Ray::set_op_delta(double op_delta)
{
    op_delta_ = op_delta;
}

void Ray::set_wvl(double wvl)
{
    wvl_ = wvl;
}

RayAtSurface Ray::at(int i) const
{
    return ray_at_srfs_[i];
}

RayAtSurface Ray::last() const
{
    return ray_at_srfs_.back();
}

void Ray::set_status(int s)
{
    status_ = s;
}

int Ray::status() const
{
    return status_;
}

void Ray::print(std::ostringstream& oss)
{
    const int idx_w = 4;
    const int val_w = 10;
    const int prec  = 4;

    int num_srfs = ray_at_srfs_.size();

    // intercept
    // headder label, S   X   Y   Z
    oss << std::setw(idx_w) << std::right << "S";
    oss << std::setw(val_w) << std::right << "X";
    oss << std::setw(val_w) << std::right << "Y";
    oss << std::setw(val_w) << std::right << "Z";
    oss << std::setw(val_w) << std::right << "L";
    oss << std::setw(val_w) << std::right << "M";
    oss << std::setw(val_w) << std::right << "N";
    oss << std::endl;

    for(int si = 0; si < num_srfs; si++)
    {
        Eigen::Vector3d intercept = ray_at_srfs_[si].intersect_pt;
        oss << std::setw(idx_w) << std::right << si;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << intercept(0);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << intercept(1);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << intercept(2);

        Eigen::Vector3d after_dir = ray_at_srfs_[si].after_dir;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << after_dir(0);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << after_dir(1);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << after_dir(2);

        oss << std::endl;
    }

    oss << std::ends;
}

void Ray::print()
{
    std::ostringstream oss;
    this->print(oss);
    std::cout << oss.str() << std::endl;
}

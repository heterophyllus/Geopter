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
    if(ray_at_srfs_.empty()){
        ray_at_srf.before = nullptr;
    }else{
        ray_at_srf.before = &ray_at_srfs_.back();
    }
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

double Ray::x(int i) const
{
    return ray_at_srfs_[i].intersect_pt(0);
}

double Ray::y(int i) const
{
    return ray_at_srfs_[i].intersect_pt(1);
}

double Ray::z(int i) const
{
    return ray_at_srfs_[i].intersect_pt(2);
}

double Ray::L(int i) const
{
    return ray_at_srfs_[i].after_dir(0);
}

double Ray::M(int i) const
{
    return ray_at_srfs_[i].after_dir(1);
}

double Ray::N(int i) const
{
    return ray_at_srfs_[i].after_dir(2);
}

double Ray::aoi(int i) const
{
    Eigen::Vector3d inc_dir;
    Eigen::Vector3d normal;

    if(i > 0) {
        inc_dir = ray_at_srfs_[i-1].after_dir;
        normal = ray_at_srfs_[i].normal;
    }else{
        inc_dir = ray_at_srfs_[0].after_dir;
        normal = ray_at_srfs_[0].normal;
    }

    // We need signed value
    double tanU1 = inc_dir(1)/inc_dir(2);
    double tanU2 = normal(1)/normal(2);
    double tanI = (tanU1 - tanU2)/(1.0 + tanU1*tanU2); // I = U1-U2
    return atan(tanI);
}

double Ray::aor(int i)
{
    Eigen::Vector3d after_dir = ray_at_srfs_[i].after_dir;
    Eigen::Vector3d normal = ray_at_srfs_[i].normal;
    double tanU1 = after_dir(1)/after_dir(2);
    double tanU2 = normal(1)/normal(2);
    double tanI_prime = (tanU1 - tanU2)/(1.0 + tanU1*tanU2);
    return atan(tanI_prime);
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

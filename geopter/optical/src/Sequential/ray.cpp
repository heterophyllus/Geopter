#include <iostream>
#include <iomanip>

#include "Sequential/ray.h"


using namespace geopter;

Ray::Ray() :
    status_(RayStatus::PassThrough),
    wvl_(0.0)
{
    //ray_at_srfs_.clear();
}

Ray::~Ray()
{
    for(auto &r : ray_at_srfs_){
        r.reset();
    }
    ray_at_srfs_.clear();
}


void Ray::prepend(std::unique_ptr<RayAtSurface> ray_at_srf)
{
    ray_at_srfs_.insert(ray_at_srfs_.begin(), std::move(ray_at_srf));
    ray_at_srfs_.front()->set_before( ray_at_srfs_[1].get() );
}


void Ray::append(const Eigen::Vector3d& inc_pt, const Eigen::Vector3d& normal, const Eigen::Vector3d& after_dir, double dist, double opl)
{
    auto ray_at_srf = std::make_unique<RayAtSurface>(inc_pt, normal, after_dir, dist, opl);
    if(ray_at_srfs_.empty()){
        ray_at_srfs_.push_back(std::move(ray_at_srf));
    }else{
        RayAtSurface *before = ray_at_srfs_.back().get();
        ray_at_srf->set_before(before);
        ray_at_srfs_.push_back(std::move(ray_at_srf));
    }
}

void Ray::append(std::unique_ptr<RayAtSurface> ray_at_srf)
{

    if(ray_at_srfs_.empty()){
        ray_at_srfs_.push_back(std::move(ray_at_srf));
    }else{
        RayAtSurface *before = ray_at_srfs_.back().get();
        ray_at_srf->set_before(before);
        ray_at_srfs_.push_back(std::move(ray_at_srf));
    }

}


void Ray::print(std::ostringstream& oss)
{
    const int idx_w = 4;
    const int val_w = 10;
    const int prec  = 4;

    int num_srfs = ray_at_srfs_.size();

    //status
    oss << "Status";
    switch (status_) {
    case RayStatus::PassThrough:
        oss << "Pass Through";
        break;
    case RayStatus::Blocked:
        oss << "Blocked";
        break;
    case RayStatus::MissedSurface:
        oss << "Missed Surface";
        break;
    case RayStatus::TotalReflection:
        oss << "Total Reflection";
        break;
    default:
        oss << "Unknown Status";
    }
    oss << std::endl;
    oss << std::endl;

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
        Eigen::Vector3d intercept = ray_at_srfs_[si]->intersect_pt();
        oss << std::setw(idx_w) << std::right << si;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << intercept(0);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << intercept(1);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << intercept(2);

        Eigen::Vector3d after_dir = ray_at_srfs_[si]->after_dir();
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << after_dir(0);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << after_dir(1);
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << after_dir(2);

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

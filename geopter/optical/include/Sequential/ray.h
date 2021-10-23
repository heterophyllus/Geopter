#ifndef RAY_H
#define RAY_H

#include <vector>
#include <sstream>
#include <memory>

#include "Eigen/Core"
#include "ray_at_surface.h"
#include "Common/geopter_error.h"

namespace geopter {


enum RayStatus
{
    PassThrough,
    Blocked,
    MissedSurface,
    TotalReflection,
};


class Ray
{
public:
    Ray();
    ~Ray();

    /** Add data at the beginning */
    void prepend(std::unique_ptr<RayAtSurface> ray_at_srf);

    /** Add data at the last */
    void append(std::unique_ptr<RayAtSurface> ray_at_srf);

    void append(const Eigen::Vector3d& inc_pt, const Eigen::Vector3d& normal, const Eigen::Vector3d& after_dir, double dist, double opl);

    inline void set_status(int s);

    inline void set_wvl(double wvl);

    inline int size() const;

    inline RayAtSurface* at(int i) const;
    inline RayAtSurface* front() const;
    inline RayAtSurface* back() const;

    inline int status() const;

    inline double wavelength() const;

    void clear();

    void print(std::ostringstream& oss);
    void print();

private:
    std::vector< std::unique_ptr<RayAtSurface> > ray_at_srfs_;
    int status_;
    double wvl_;
    int array_size_;
};


int Ray::size() const
{
    assert(array_size_ == (int)ray_at_srfs_.size());
    return array_size_;
}

int Ray::status() const
{
    return status_;
}

double Ray::wavelength() const
{
    return wvl_;
}

RayAtSurface* Ray::at(int i) const
{    
    assert(array_size_ == (int)ray_at_srfs_.size());

    if(i < array_size_){
        return ray_at_srfs_[i].get();
    }else{
        throw RayOutOfRangeError();
    }
}

RayAtSurface* Ray::front() const
{
    return ray_at_srfs_.front().get();
}

RayAtSurface* Ray::back() const
{
    return ray_at_srfs_.back().get();
}

void Ray::set_wvl(double wvl)
{
    wvl_ = wvl;
}

void Ray::set_status(int s)
{
    status_ = s;
}

} //namespace geopter

#endif // RAY_H

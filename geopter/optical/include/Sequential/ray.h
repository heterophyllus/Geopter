#ifndef RAY_H
#define RAY_H

#include <vector>
#include <sstream>
#include <memory>
#include <cassert>

#include "Eigen/Core"
#include "ray_at_surface.h"

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

    int size() const;

    void clear();

    /** Add data at the beginning */
    void prepend(std::shared_ptr<RayAtSurface> ray_at_srf);

    /** Add data at the last */
    void append(std::shared_ptr<RayAtSurface> ray_at_srf);

    void set_status(int s);

    void set_wvl(double wvl);

    inline RayAtSurface* at(int i) const;
    inline RayAtSurface* front() const;
    inline RayAtSurface* back() const;

    int status() const;

    double wavelength() const;

    void print(std::ostringstream& oss);
    void print();

private:
    std::vector< std::shared_ptr<RayAtSurface> > ray_at_srfs_;
    double wvl_;
    int status_;
};



RayAtSurface* Ray::at(int i) const
{
    if(i < (int)ray_at_srfs_.size()){
        return ray_at_srfs_[i].get();
    }else{
        return nullptr;
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

} //namespace geopter

#endif // RAY_H

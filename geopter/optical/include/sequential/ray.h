/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
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

#ifndef RAY_H
#define RAY_H

#include <vector>
#include <sstream>
#include <memory>
#include <cassert>

#include "ray_at_surface.h"
#include "common/geopter_error.h"

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
    Ray(int n);
    ~Ray();

    RayAtSurface* operator[](std::size_t n) & {return ray_at_srfs_[n].get();}

    void allocate(int n);

    /** Add data at the beginning */
    void prepend(std::unique_ptr<RayAtSurface> ray_at_srf);

    /** Add data at the last */
    void append(const Eigen::Vector3d& inc_pt, const Eigen::Vector3d& normal, const Eigen::Vector3d& after_dir, double dist, double opl);

    inline void set_status(int s);
    inline void set_wvl(double wvl);
    inline void set_pupil_coord(const Eigen::Vector2d& pupil);

    void set_reached_surface(int i);

    inline int size() const;

    inline int reached_surface() const;

    inline RayAtSurface* at(int i);
    inline RayAtSurface* front() const;
    inline RayAtSurface* back() const;
    inline RayAtSurface* at_lens_back() const;
    inline int status() const;
    inline double wavelength() const;
    inline const Eigen::Vector2d& pupil_coord() const;

    double optical_path_length() const;

    void clear();

    void print(std::ostringstream& oss);
    void print();

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

private:
    std::vector< std::unique_ptr<RayAtSurface> > ray_at_srfs_;
    int status_;
    double wvl_;
    double opl_;
    int size_;
    int reached_surface_index_;
    Eigen::Vector2d pupil_crd_;
};

using RayPtr = std::shared_ptr<Ray>;


int Ray::size() const
{
    return (int)ray_at_srfs_.size();
}

int Ray::reached_surface() const
{
    return reached_surface_index_;
}

int Ray::status() const
{
    return status_;
}

double Ray::wavelength() const
{
    return wvl_;
}

RayAtSurface* Ray::at(int i)
{    
    assert(size_ == (int)ray_at_srfs_.size());
    if(i < size_){
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

RayAtSurface* Ray::at_lens_back() const
{
    /*
    auto itr = ray_at_srfs_.end();
    itr--;
    return itr->get();
    */


    int len = ray_at_srfs_.size();
    int lens_back_index = len - 1 -1;
    return ray_at_srfs_[lens_back_index].get();

}

void Ray::set_wvl(double wvl)
{
    wvl_ = wvl;
}

void Ray::set_status(int s)
{
    status_ = s;
}

void Ray::set_pupil_coord(const Eigen::Vector2d &pupil)
{
    pupil_crd_ = pupil;
}

const Eigen::Vector2d& Ray::pupil_coord() const
{
    return pupil_crd_;
}

} //namespace geopter

#endif // RAY_H

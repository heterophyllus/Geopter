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

#include "ray_segment.h"
#include "common/geopter_error.h"

namespace geopter {

class Ray
{
public:
    Ray();
    Ray(int n);
    ~Ray();

    void allocate(int n);

    /** Add data at the beginning */
    void prepend(std::unique_ptr<RaySegment> ray_at_srf);

    /** Add data at the last */
    void append(const Eigen::Vector3d& inc_pt, const Eigen::Vector3d& normal, const Eigen::Vector3d& after_dir, double dist, double opl);

    void set_status(TraceError s) {status_ = s;}
    void set_wvl(double wvl) { wvl_ = wvl; }
    void set_pupil_coord(const Eigen::Vector2d& pupil) { pupil_crd_ = pupil;}

    void set_reached_surface(int i);

    int size() const { return ray_at_srfs_.size();}

    int reached_surface() const { return reached_surface_index_; }

    RaySegment* at(int i) { return ray_at_srfs_[i].get(); }
    RaySegment* front() const { return ray_at_srfs_.front().get();}
    RaySegment* back() const { return ray_at_srfs_.back().get();}
    RaySegment* at_lens_back() const { int len = ray_at_srfs_.size(); return ray_at_srfs_[len-2].get();}
    TraceError status() const { return status_;}
    double wavelength() const { return wvl_;}
    const Eigen::Vector2d& pupil_coord() const { return pupil_crd_;}

    double optical_path_length() const;

    void clear();

    void print(std::ostringstream& oss);
    void print();

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

private:
    std::vector< std::unique_ptr<RaySegment> > ray_at_srfs_;
    TraceError status_;
    double wvl_;
    double opl_;
    int size_;
    int reached_surface_index_;
    Eigen::Vector2d pupil_crd_;
};

using RayPtr = std::shared_ptr<Ray>;


} //namespace geopter

#endif // RAY_H

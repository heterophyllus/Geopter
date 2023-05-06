/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
** 
** This file is part of Geopter.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public
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

    void Allocate(int n);

    /** Add data at the beginning */
    void Prepend(std::unique_ptr<RaySegment> ray_at_srf);

    /** Add data at the last */
    void Append(const Eigen::Vector3d& inc_pt, const Eigen::Vector3d& normal, const Eigen::Vector3d& after_dir, double dist, double opl);

    void SetStatus(TraceError s) {status_ = s;}
    void SetWavelength(double wvl) { wvl_ = wvl; }
    void SetPupilCoordinate(const Eigen::Vector2d& pupil) { pupil_crd_ = pupil;}

    void SetReachedSurfaceIndex(int i);

    int NumberOfSegments() const { return segments_.size();}

    int GetReachedSurfaceIndex() const { return reached_surface_index_; }

    RaySegment* GetSegmentAt(int i) { return segments_[i].get(); }
    RaySegment* GetFront() const { return segments_.front().get();}
    RaySegment* GetBack() const { return segments_.back().get();}
    RaySegment* GetLensBack() const { int len = segments_.size(); return segments_[len-2].get();}
    TraceError Status() const { return status_;}
    double Savelength() const { return wvl_;}
    const Eigen::Vector2d& PupilCoordinate() const { return pupil_crd_;}
    double Wavelength() const { return wvl_;}

    double OpticalPathLength() const;

    void Clear();

    void Print(std::ostringstream& oss);
    void Print();

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

private:
    std::vector< std::unique_ptr<RaySegment> > segments_;
    TraceError status_;
    double wvl_;
    double opl_;
    int num_segments_;
    int reached_surface_index_;
    Eigen::Vector2d pupil_crd_;
};

using RayPtr = std::shared_ptr<Ray>;


} //namespace geopter

#endif // RAY_H

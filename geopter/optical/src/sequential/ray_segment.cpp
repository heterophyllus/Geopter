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
** You should have received a copy of the GNU General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com
**             Date: May 16th, 2021
********************************************************************************/


#include "sequential/ray_segment.h"

using namespace geopter;

RaySegment::RaySegment()
{
    intersect_pt_ = Eigen::Vector3d::Zero(3);
    normal_       = Eigen::Vector3d::Zero(3);
    after_dir_    = Eigen::Vector3d::Zero(3);
    path_length_  = 0.0;
    opl_  = 0.0;
    before_ = nullptr;
    status_ = TRACE_NOT_REACHED_ERROR;
}

RaySegment::RaySegment(int i, const Eigen::Vector3d& inc_pt, const Eigen::Vector3d& normal, const Eigen::Vector3d& after_dir, double dist, double opl, RaySegment* before)
{
    index_        = i;
    intersect_pt_ = inc_pt;
    normal_       = normal;
    after_dir_    = after_dir;
    path_length_  = dist;
    opl_          = opl;
    before_       = before;
    status_       = TRACE_NOT_REACHED_ERROR;
}

RaySegment::RaySegment(const RaySegment& other)
{
    intersect_pt_ = other.IntersectPt();
    normal_       = other.SurfaceNormal();
    after_dir_    = other.Direction();
    path_length_  = other.PathLength();
    opl_          = other.OpticalPathLength();
    before_       = nullptr;
    status_       = other.Status();
}

RaySegment::~RaySegment()
{
    before_ = nullptr;
}

void RaySegment::SetData(const Eigen::Vector3d &inc_pt, const Eigen::Vector3d &normal, const Eigen::Vector3d &after_dir, double dist, double opl)
{
    intersect_pt_ = inc_pt;
    normal_       = normal;
    after_dir_    = after_dir;
    path_length_  = dist;
    opl_          = opl;
}

double RaySegment::AngleOfIncidence() const
{
    Eigen::Vector3d inc_dir;

    if(before_) {
        inc_dir = before_->Direction();
    }else{
        inc_dir = after_dir_;
    }

    // We need signed value
    double tanU1 = inc_dir(1)/inc_dir(2);
    double tanU2 = normal_(1)/normal_(2);
    double tanI = (tanU1 - tanU2)/(1.0 + tanU1*tanU2); // I = U1-U2
    return atan(tanI);
}

double RaySegment::AngleOfRefraction() const
{
    double tanU1 = after_dir_(1)/after_dir_(2);
    double tanU2 = normal_(1)/normal_(2);
    double tanI_prime = (tanU1 - tanU2)/(1.0 + tanU1*tanU2);
    return atan(tanI_prime);
}

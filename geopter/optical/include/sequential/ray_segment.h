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


#ifndef RAY_SEGMENT_H
#define RAY_SEGMENT_H

#include "Eigen/Core"
#include "sequential/trace_error.h"

namespace geopter {

/** Ray data on a surface */
class RaySegment
{
public:
    RaySegment();
    RaySegment(int i, const Eigen::Vector3d& inc_pt, const Eigen::Vector3d& normal, const Eigen::Vector3d& after_dir, double dist, double opl, RaySegment* before);
    RaySegment(const RaySegment& other);
    ~RaySegment();

    void set_data(const Eigen::Vector3d& inc_pt, const Eigen::Vector3d& normal, const Eigen::Vector3d& after_dir, double dist, double opl);

    void set_before(RaySegment* before) { before_ = before;}

    void set_index(int i) { index_ = i; }

    void set_status(TraceError s) { status_ = s;}

    const Eigen::Vector3d& intersect_pt() const { return intersect_pt_;}

    double path_length() const { return path_length_;}

    /** optical path length from the previous surface to the current */
    double optical_path_length() const { return opl_;}

    /** Local coordinate at the intersection point */
    double x() const { return intersect_pt_(0);}
    double y() const { return intersect_pt_(1);}
    double z() const { return intersect_pt_(2);}

    const Eigen::Vector3d& after_dir() const { return after_dir_;}
    double L() const { return after_dir_(0);}
    double M() const { return after_dir_(1);}
    double N() const { return after_dir_(2);}

    const Eigen::Vector3d& surface_normal() const { return normal_;}
    double srl() const { return normal_(0);}
    double srm() const { return normal_(1);}
    double srn() const { return normal_(2);}

    /** Ray height from the axis */
    double height() const { return sqrt( pow(intersect_pt_(0),2) + pow(intersect_pt_(1),2) );}

    /** Angle of incidence (signed) */
    double aoi() const;

    /** Angle of refraction (signed) */
    double aor() const;

    TraceError status() const { return status_; }

    /** Get surface index of this ray segment */
    int index() const { return index_; }

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

private:

    /** surface index */
    int index_;

    /** ray intersect point on the surface */
    Eigen::Vector3d intersect_pt_;

    /** surface normal at the intersect point */
    Eigen::Vector3d normal_;

    /** directional cosine after surface interaction */
    Eigen::Vector3d after_dir_;

    /** distance from the previous point to the intersect point */
    double path_length_;

    /** optical path length from the previous to the current */
    double opl_;

    TraceError status_;

    RaySegment* before_;

};


} //namespace geopter

#endif //RAY_AT_SURFACE_H

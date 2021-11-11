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

//============================================================================
/// \file   ray_at_surface.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#ifndef RAY_AT_SURFACE_H
#define RAY_AT_SURFACE_H

#include "Eigen/Core"

namespace geopter {

/** Ray parameters on the surface */
class RayAtSurface
{
public:
    RayAtSurface();
    RayAtSurface(const Eigen::Vector3d& inc_pt, const Eigen::Vector3d& normal, const Eigen::Vector3d& after_dir, double dist, double opl, RayAtSurface *before=nullptr);

    ~RayAtSurface();

    inline void set_before(RayAtSurface* before);

    inline Eigen::Vector3d intersect_pt() const;

    inline double distance_from_before() const;

    /** optical path length from the previous to the current */
    inline double optical_path_length() const;

    /** Local coordinate at the intersection point */
    inline double x() const;
    inline double y() const;
    inline double z() const;

    inline Eigen::Vector3d after_dir() const;
    inline double L() const;
    inline double M() const;
    inline double N() const;

    inline Eigen::Vector3d surface_normal() const;
    inline double srl() const;
    inline double srm() const;
    inline double srn() const;

    /** Ray height from the axis */
    inline double height() const;

    /** Angle of incidence (signed) */
    double aoi() const;

    /** Angle of refraction (signed) */
    double aor() const;


private:
    /** ray intersect point on the surface */
    Eigen::Vector3d intersect_pt_;

    /** surface normal at the intersect point */
    Eigen::Vector3d normal_;

    /** directional cosine after surface interaction */
    Eigen::Vector3d after_dir_;

    /** distance from the previous point to the intersect point */
    double distance_from_before_;

    /** optical path length from the previous to the current */
    double opl_;

    RayAtSurface* before_;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


void RayAtSurface::set_before(RayAtSurface *before)
{
    before_ = before;
}

Eigen::Vector3d RayAtSurface::intersect_pt() const
{
    return intersect_pt_;
}

double RayAtSurface::x() const
{
    return intersect_pt_(0);
}

double RayAtSurface::y() const
{
    return intersect_pt_(1);
}

double RayAtSurface::z() const
{
    return intersect_pt_(2);
}

double RayAtSurface::height() const
{
    return sqrt( pow(intersect_pt_(0),2) + pow(intersect_pt_(1),2) );
}

double RayAtSurface::distance_from_before() const
{
    return distance_from_before_;
}

Eigen::Vector3d RayAtSurface::after_dir() const
{
    return after_dir_;
}

double RayAtSurface::L() const
{
    return after_dir_(0);
}

double RayAtSurface::M() const
{
    return after_dir_(1);
}

double RayAtSurface::N() const
{
    return after_dir_(2);
}

Eigen::Vector3d RayAtSurface::surface_normal() const
{
    return normal_;
}

double RayAtSurface::srl() const
{
    return normal_(0);
}

double RayAtSurface::srm() const
{
    return normal_(1);
}

double RayAtSurface::srn() const
{
    return normal_(2);
}

double RayAtSurface::optical_path_length() const
{
    return opl_;
}

} //namespace geopter

#endif //RAY_AT_SURFACE_H

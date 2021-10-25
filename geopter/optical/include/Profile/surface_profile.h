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
/// \file   surface_profile.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#ifndef SURFACEPROFILE_H
#define SURFACEPROFILE_H


#include <vector>
#include "Eigen/Core"

namespace geopter {

class SurfaceProfile
{
public:

    enum Type
    {
        Sphere,
        EvenAsphere,
        OddAsphere
    };

    SurfaceProfile();
    virtual ~SurfaceProfile();

    virtual std::string name() const;

    virtual void set_cv(double c);

    /** Returns center curvature */
    virtual double cv() const;

    virtual void set_radius(double r);

    /** Returns center radius */
    virtual double radius() const;

    /** Returns the value of the profile function at point @param{p} */
    virtual double f(const Eigen::Vector3d& p) const;

    /** Returns the gradient of the profile function at point *p* */
    virtual Eigen::Vector3d df(const Eigen::Vector3d& p) const;

    /** Returns the unit normal of the profile at point *p* */
    virtual Eigen::Vector3d normal(const Eigen::Vector3d& p) const;

    /** Returns the sagitta (z coordinate) of the surface at x, y */
    virtual double sag(double x, double y) const;

    virtual double deriv_1st(double h) const;
    virtual double deriv_2nd(double h) const;


    /**
     * @brief Intersect a profile, starting from an arbitrary point
     * @param p0 start point of the ray in the profile's coordinate system
     * @param d direction cosine of the ray in the profile's coordinate system
     * @param eps numeric tolerance for convergence of any iterative procedure
     * @param z_dir +1 if propagation positive direction, -1 if otherwise
     */
    virtual void intersect(Eigen::Vector3d& pt, double& s, const Eigen::Vector3d& p0, const Eigen::Vector3d& d, double eps=1.0e-12, double z_dir=1.0);

    /**
     * @brief Intersect a profile, starting from an arbitrary point
     * @note From Spencer and Murty, General Ray-Tracing Procedure <https://doi.org/10.1364/JOSA.52.000672>
     * @param pt intersect point
     * @param distance to the intersect point
     * @param p0 start point of the ray in the profile's coordinate system
     * @param d direction cosine of the ray in the profile's coordinate system
     * @param eps numeric tolerance for convergence of any iterative procedure
     * @param z_dir +1 if propagation positive direction, -1 if otherwise
     */
    virtual void intersect_spencer(Eigen::Vector3d& pt, double& s, const Eigen::Vector3d& p0, const Eigen::Vector3d& d, double eps=1.0e-12, double z_dir=1.0);


protected:
    double cv_;
    std::string name_;
};

} //namespace geopter

#endif // SURFACEPROFILE_H

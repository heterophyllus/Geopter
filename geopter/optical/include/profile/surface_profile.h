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

#ifndef SURFACEPROFILE_H
#define SURFACEPROFILE_H

#include <sstream>
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

    inline std::string name() const;

    inline void set_cv(double c);

    /** Returns center curvature */
    inline double cv() const;

    void set_radius(double r);

    /** Returns center radius */
    double radius() const;

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
     * @param dir direction cosine of the ray in the profile's coordinate system
     */
    virtual bool intersect(Eigen::Vector3d& pt, double& distance, const Eigen::Vector3d& p0, const Eigen::Vector3d& dir);

    /**
     * @brief Print coefficinet data
     * @param oss
     */
    virtual void print(std::ostringstream& oss);

protected:
    double cv_;
    std::string name_;
    double eps_;
};

std::string SurfaceProfile::name() const
{
    return name_;
}

double SurfaceProfile::cv() const
{
    return cv_;
}

void SurfaceProfile::set_cv(double c)
{
    cv_ = c;
}



} //namespace geopter

#endif // SURFACEPROFILE_H

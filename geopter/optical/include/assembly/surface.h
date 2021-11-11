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

#ifndef SURFACE_H
#define SURFACE_H

#include "assembly/interface.h"

namespace geopter {

/** Optical refractive surface */
class Surface : public Interface
{
public:
    Surface(std::string lbl="");
    Surface(double r);
    ~Surface();


    /**
     * @brief Compute intersect point and distance from the previous interface
     * @param pt intersect point
     * @param s distance from the previous interface
     * @param p0
     * @param d
     * @param eps precision
     * @param z_dir
     */
    void intersect(Eigen::Vector3d& pt, double& s, const Eigen::Vector3d& p0, const Eigen::Vector3d& d, double eps=1.0e-12, double z_dir=1.0);

    Eigen::Vector3d normal(Eigen::Vector3d p);

    void print();
    void print(std::ostringstream& oss);

protected:


};

} //namespace geopter

#endif // SURFACE_H

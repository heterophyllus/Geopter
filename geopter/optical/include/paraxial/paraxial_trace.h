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

#ifndef PARAXIALTRACE_H
#define PARAXIALTRACE_H

#include "paraxial_path.h"
#include "paraxial_ray.h"
#include "system/optical_system.h"

namespace geopter {


class ParaxialTrace
{
public:
    ParaxialTrace(const OpticalSystem* sys);
    ~ParaxialTrace();

    /** 
     * @brief Do paraxial trace throughout the system
     * @param par_path paraxial path
     * @param y0 paraxial ray height at starting point
     * @param u0 slope of outgoing ray at starting point
     */
    std::shared_ptr<ParaxialRay> trace_paraxial_ray_from_object(double y0, double u0, double wvl);

    std::shared_ptr<ParaxialRay> trace_paraxial_axis_ray(double wvl);

    std::shared_ptr<ParaxialRay> trace_paraxial_chief_ray(double wvl);

    /**
     * @brief Generate the paraxial path 
     * 
     * @param start start surface index
     * @param end end surface index
     * @param wi wavelength index
     * @return ParaxialPath 
     */
    ParaxialPath paraxial_path(int start, int end, double wvl) const;

    void get_starting_coords(double *y0, double *u0, double *ybar0, double *ubar0) const;

    Eigen::Matrix2d system_matrix(int s1, int s2, double wvl) const;

    void compute_first_order_data(FirstOrderData* fod, double wvl);

private:
    ParaxialPath forward_paraxial_path(int start, int end, double wvl) const;
    ParaxialPath reverse_paraxial_path(int start, int end, double wvl) const;

    void compute_starting_data();

    const OpticalSystem* opt_sys_;

    double ref_u0_;
    double ref_y0_;
    double ref_ubar0_;
    double ref_ybar0_;
};

} // namespace geopter

#endif // PARAXIALTRACE_H

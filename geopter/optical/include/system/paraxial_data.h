/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi All Rights Reserved.
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
**             Date: November 10th, 2021
********************************************************************************/

#ifndef PARAXIAL_DATA_H
#define PARAXIAL_DATA_H

#include <memory>
#include <sstream>
#include "paraxial/paraxial_ray.h"


namespace geopter{

class OpticalSystem;

/** Container for optical data based on paraxial ray tracing */
class ParaxialData
{
public:
    ParaxialData();
    ~ParaxialData();

    inline std::shared_ptr<ParaxialRay> axial_ray(int wi) const;
    inline std::shared_ptr<ParaxialRay> principle_ray(int wi) const;
    inline double reduction_rate() const;
    inline double entrance_pupil_distance() const;
    inline double entrance_pupil_radius() const;
    inline double exit_pupil_distance() const;
    inline double exit_pupil_radius() const;
    inline double object_distance() const;
    inline double image_distance() const;
    inline double optical_invariant() const;
    inline double effective_focal_length() const;
    inline double f_number() const;
    inline double front_focal_length() const;
    inline double back_focal_length() const;
    inline double object_angle() const;
    inline double image_height() const;
    inline double object_space_NA() const;
    inline double image_space_NA() const;

    double effective_focal_length(int s1, int s2, int wi) const;

    void update(OpticalSystem* opt_sys);

    void print(std::ostringstream& oss);

private:
    void clear();

    /** parallel to axis at s1 */
    std::shared_ptr<ParaxialRay> p_ray_;

    /** with slope at s1 */
    std::shared_ptr<ParaxialRay> q_ray_;

    /** paraxial axial rays computed with all wavelengths */
    std::vector<std::shared_ptr<ParaxialRay>> ax_rays_;

    /** paraxial principle rays computed with all wavelengths */
    std::vector<std::shared_ptr<ParaxialRay>> pr_rays_;

    double opt_inv_;
    double efl_;
    double fno_;
    double red_;
    double enp_dist_;
    double enp_radius_;
    double exp_dist_;
    double exp_radius_;
    double img_dist_;
    double obj_dist_;
    double pp1_;
    double ppk_;
    double ffl_;
    double bfl_;
    double n_obj_;
    double n_img_;
    double img_ht_;
    double obj_ang_;
    double obj_na_;
    double img_na_;
};

std::shared_ptr<ParaxialRay> ParaxialData::axial_ray(int wi) const
{
    return ax_rays_[wi];
}

std::shared_ptr<ParaxialRay> ParaxialData::principle_ray(int wi) const
{
    return pr_rays_[wi];
}

double ParaxialData::reduction_rate() const
{
    return red_;
}

double ParaxialData::entrance_pupil_distance() const
{
    return enp_dist_;
}

double ParaxialData::entrance_pupil_radius() const
{
    return enp_radius_;
}

double ParaxialData::exit_pupil_distance() const
{
    return exp_dist_;
}

double ParaxialData::exit_pupil_radius() const
{
    return exp_radius_;
}

double ParaxialData::object_distance() const
{
    return obj_dist_;
}

double ParaxialData::image_distance() const
{
    return img_dist_;
}


double ParaxialData::optical_invariant() const
{
    return opt_inv_;
}

double ParaxialData::effective_focal_length() const
{
    return efl_;
}

double ParaxialData::f_number() const
{
    return fno_;
}

double ParaxialData::front_focal_length() const
{
    return ffl_;
}

double ParaxialData::back_focal_length() const
{
    return bfl_;
}

double ParaxialData::object_angle() const
{
    return obj_ang_;
}

double ParaxialData::image_height() const
{
    return img_ht_;
}

double ParaxialData::object_space_NA() const
{
    return obj_na_;
}

double ParaxialData::image_space_NA() const
{
    return img_na_;
}

} // namespace geopter


#endif //PARAXIAL_DATA_H

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

#ifndef SEQUENTIALTRACE_H
#define SEQUENTIALTRACE_H


#include "system/optical_system.h"
#include "sequential/sequential_path.h"
#include "sequential/ray.h"
#include "sequential/trace_error.h"
#include "data/grid_array.h"
#include "data/hexapolar_array.h"

namespace geopter {

class SequentialTrace
{
public:
    SequentialTrace(OpticalSystem* sys);
    ~SequentialTrace();

    /** Base function for ray tracing. Trace a ray throughout the given sequantial path */
    TraceError trace_ray_throughout_path(RayPtr ray, const SequentialPath& seq_path, const Eigen::Vector3d& pt0, const Eigen::Vector3d& dir0);


    /** Trace a single ray at the given pupil coordinate */
    TraceError trace_pupil_ray(RayPtr ray, const SequentialPath& seq_path, const Eigen::Vector2d& pupil_crd, const Field* fld, double wvl);

    /** Trace reference rays(chief, meridional upper/lower, sagittal upper/lower */
    bool trace_reference_rays(std::vector<std::shared_ptr<Ray>>& ref_rays, const Field* fld, double wvl);


    /** Trace chief ray and returns x/y focus shift */
    Eigen::Vector2d trace_coddington(const Field* fld, double wvl);

    bool trace_coddington(Eigen::Vector2d& s_t, const std::shared_ptr<Ray> ray, const SequentialPath& path);

    bool search_ray_aiming_at_surface(RayPtr ray, Eigen::Vector2d& aim_pt, const Field* fld, int target_srf_idx, const Eigen::Vector2d& xy_target);

    bool aim_chief_ray(Eigen::Vector2d& aim_pt, Eigen::Vector3d& obj_pt, const Field* fld, double wvl);

    /**  Refract incoming direction, d_in, about normal */
    bool bend(Eigen::Vector3d& d_out, const Eigen::Vector3d& d_in, const Eigen::Vector3d& normal, double n_in, double n_out);

    /** Get object coordinate for the given field */
    Eigen::Vector3d get_default_object_pt(const Field* fld);

    /** Get sequential path between start and end */
    SequentialPath sequential_path(int start, int end, double wvl);

    /** Get overall sequential path object from object to image */
    SequentialPath sequential_path(double wvl);


    double compute_vignetting_factor_for_pupil(const Eigen::Vector2d& full_pupil, const Field& fld);

    std::vector<double> compute_vignetting_factors(const Field& fld);

    inline void set_aperture_check(bool state);
    inline bool aperture_check_state() const;

    inline void set_apply_vig(bool state);
    inline bool apply_vig_status() const;

private:
    void pupil_coord_to_obj(Eigen::Vector3d& pt0, Eigen::Vector3d& dir0, const Eigen::Vector2d& pupil_crd, const Field* fld);
    
    OpticalSystem *opt_sys_;

    bool do_aperture_check_;
    bool do_apply_vig_;
};


void SequentialTrace::set_aperture_check(bool state)
{
    do_aperture_check_ = state;
}

bool SequentialTrace::aperture_check_state() const
{
    return do_aperture_check_;
}

void SequentialTrace::set_apply_vig(bool state)
{
    do_apply_vig_ = state;
}

bool SequentialTrace::apply_vig_status() const
{
    return do_apply_vig_;
}


}

#endif // SEQUENTIALTRACE_H

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
#include "data/grid_array.h"
#include "data/hexapolar_array.h"

namespace geopter {

class SequentialTrace
{
public:
    SequentialTrace(OpticalSystem* sys);
    ~SequentialTrace();

    /** Base function for ray tracing. Trace a ray throughout the given sequantial path */
    RayPtr trace_ray_throughout_path(const SequentialPath& seq_path, const Eigen::Vector3d& pt0, const Eigen::Vector3d& dir0);

    void trace_ray_throughout_path(RayPtr ray, const SequentialPath& seq_path, const Eigen::Vector3d& pt0, const Eigen::Vector3d& dir0);


    /** Trace a single ray at the given pupil coordinate */
    RayPtr trace_pupil_ray(const Eigen::Vector2d& pupil_crd, const Field* fld, double wvl);

    void trace_pupil_ray(RayPtr ray, const SequentialPath& seq_path, const Eigen::Vector2d& pupil_crd, const Field* fld, double wvl);

    /** Trace reference rays(chief, meridional upper/lower, sagittal upper/lower */
    void trace_reference_rays(std::vector<std::shared_ptr<Ray>>& ref_rays, const Field* fld, double wvl);

    /** Trace grid pattern rays */
    GridArray< RayPtr > trace_grid_rays(const Field* fld, double wvl, int nrd=21);


    /** Trace hexapolar pattern rays */
    HexapolarArray<RayPtr> trace_hexapolar_rays(const Field* fld, double wvl, int nrd=21);


    /** Trace chief ray and returns x/y focus shift */
    Eigen::Vector2d trace_coddington(const Field* fld, double wvl, double offset=0.0);

    Eigen::Vector2d aim_chief_ray(const Field* fld, double wvl);

    /**
     * @brief Search aim point of the ray to xy_target on the given surface
     * 
     * @param srf_idx target surface index
     * @param xy_target target coordinate
     * @param fi field index
     * @param wi wavelength index
     * @return Eigen::Vector2d aim point on paraxial entrance pupil plane
     */
    Eigen::Vector2d search_aim_point(int srf_idx, const Eigen::Vector2d& xy_target, const Field* fld, double wvl);

    /**  Refract incoming direction, d_in, about normal */
    Eigen::Vector3d bend(const Eigen::Vector3d& d_in, const Eigen::Vector3d& normal, double n_in, double n_out);

    /** Get object coordinate for the given field */
    Eigen::Vector3d object_coord(const Field* fld);

    /** Get sequential path between start and end */
    SequentialPath sequential_path(int start, int end, double wvl);

    /** Get overall sequential path object from object to image */
    SequentialPath sequential_path(double wvl);


    double compute_vignetting_factor_for_pupil(const Eigen::Vector2d& full_pupil, const Field& fld);

    std::vector<double> compute_vignetting_factors(const Field& fld);

    void set_aperture_check(bool state);
    bool aperture_check_state() const;

    void set_apply_vig(bool state);
    bool apply_vig_status() const;

private:
    double y_stop_coordinate(double y1, int ifcx, const Eigen::Vector3d& pt0, double dist, double wvl, double y_target);

    void pupil_coord_to_obj(Eigen::Vector3d& pt0, Eigen::Vector3d& dir0, const Eigen::Vector2d& pupil_crd, const Field* fld);
    
    OpticalSystem *opt_sys_;
    FundamentalData fund_data_;

    bool do_aperture_check_;
    bool do_apply_vig_;
};

}

#endif // SEQUENTIALTRACE_H

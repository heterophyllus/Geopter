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

namespace geopter {

class SequentialTrace
{
public:
    SequentialTrace(OpticalSystem* sys);
    ~SequentialTrace();

    /** Base function for ray tracing. Trace a ray throughout the given sequantial path */
    TraceError TraceRayThroughoutPath(RayPtr ray, const SequentialPath& seq_path, const Eigen::Vector3d& pt0, const Eigen::Vector3d& dir0);

    /** Trace a single ray at the given pupil coordinate */
    TraceError TracePupilRay(RayPtr ray, const SequentialPath& seq_path, const Eigen::Vector2d& pupil_crd, const Field* fld, double wvl);

    RayPtr CreatePupilRay(const Eigen::Vector2d& pupil_crd, const Field* fld, double wvl);

    /** Trace reference rays(chief, meridional upper/lower, sagittal upper/lower */
    bool TraceReferenceRays(std::vector<std::shared_ptr<Ray>>& ref_rays, const Field* fld, double wvl);

    /**
     * @brief Trace chief ray using Coddington equation
     * @param s_t x/y focus shift
     * @param ray chief ray
     * @param path sequential path
     * @return true if no error
     */
    bool TraceCoddington(Eigen::Vector2d& s_t, const std::shared_ptr<Ray> ray, const SequentialPath& path);

    bool SearchRayAimingAtSurface(RayPtr ray, Eigen::Vector2d& aim_pt, const Field* fld, int target_srf_idx, const Eigen::Vector2d& xy_target);

    bool AimChiefRay(Eigen::Vector2d& aim_pt, Eigen::Vector3d& obj_pt, const Field* fld, double wvl);

    /**  Refract incoming direction, d_in, about normal */
    bool Bend(Eigen::Vector3d& d_out, const Eigen::Vector3d& d_in, const Eigen::Vector3d& normal, double n_in, double n_out);

    /** Get object coordinate for the given field */
    Eigen::Vector3d GetDefaultObjectPt(const Field* fld);

    /** Get sequential path between start and end */
    SequentialPath CreateSequentialPath(int start, int end, double wvl);

    /** Get overall sequential path object from object to image */
    SequentialPath CreateSequentialPath(double wvl);


    double ComputeVignettingFactorForPupil(const Eigen::Vector2d& full_pupil, const Field& fld);

    std::vector<double> ComputeVignettingFactors(const Field& fld);

    void SetApertureCheck(bool state) {do_aperture_check_ = state;}
    bool ApertureCheckState() const { return do_aperture_check_;}

    void SetApplyVig(bool state) { do_apply_vig_ = state;}
    bool ApplyVigStatus() const { return do_apply_vig_;}

private:
    void ConvertCoordinatePupilToObj(Eigen::Vector3d& pt0, Eigen::Vector3d& dir0, const Eigen::Vector2d& pupil_crd, const Field* fld);
    
    OpticalSystem *opt_sys_;

    bool do_aperture_check_;
    bool do_apply_vig_;
};




}

#endif // SEQUENTIALTRACE_H

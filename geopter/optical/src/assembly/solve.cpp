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
**             Date: May 4th, 2022
********************************************************************************/

#include "assembly/solve.h"
#include "system/optical_system.h"
#include "paraxial/paraxial_trace.h"

using namespace geopter;


EdgeThicknessSolve::EdgeThicknessSolve(int gap_index, double height, double value) :
    Solve()
{
    solve_type_ = SolveType::EdgeThickness;
    gap_index_ = gap_index;
    height_ = height;
    value_ = value;
}

bool EdgeThicknessSolve::check(const OpticalSystem *opt_sys)
{
    if(gap_index_ < 0 || height_ < 0.0 || value_ < 0.0) return false;

    if(gap_index_ > opt_sys->optical_assembly()->gap_count()-1) return false;

    return true;
}

void EdgeThicknessSolve::apply(OpticalSystem* opt_sys)
{
    int s1 = gap_index_;
    int s2 = s1+1;
    double sag1 = opt_sys->optical_assembly()->surface(s1)->sag(0.0, height_);
    double sag2 = opt_sys->optical_assembly()->surface(s2)->sag(0.0, height_);

    double thi = value_ - ( -sag1 + sag2); //et = -sag1 + thi + sag2;

    opt_sys->optical_assembly()->gap(gap_index_)->set_thi(thi);
}

OverallLengthSolve::OverallLengthSolve(int gi, double value, int s1, int s2) : Solve()
{
    solve_type_ = SolveType::OverallLength;
    surface1_ = s1;
    surface2_ = s2;
    gap_index_ = gi;
    value_ = value;
}

bool OverallLengthSolve::check(const OpticalSystem *opt_sys)
{
    if(surface1_ >= surface2_) return false;
    if(surface2_ > opt_sys->optical_assembly()->surface_count()-1) return false;
    if(gap_index_ > opt_sys->optical_assembly()->gap_count()-1) return false;

    return true;
}

void OverallLengthSolve::apply(OpticalSystem *opt_sys)
{
    double cur_oal = opt_sys->optical_assembly()->overall_length(surface1_, surface2_);
    double cur_thi = opt_sys->optical_assembly()->gap(gap_index_)->thi();
    double new_thi = (value_ - cur_oal) + cur_thi;

    opt_sys->optical_assembly()->gap(gap_index_)->set_thi(new_thi);
}


MarginalHeightSolve::MarginalHeightSolve(int gi, double value, double zone) :
    Solve()
{
    solve_type_ = SolveType::MarginalHeight;
    gap_index_ = gi;
    height_ = value;
    pupil_zone_ = zone;
}

bool MarginalHeightSolve::check(const OpticalSystem * /*opt_sys*/)
{
    return true;
}

void MarginalHeightSolve::apply(OpticalSystem* opt_sys)
{
    const double ref_wvl = opt_sys->optical_spec()->spectral_region()->reference_wvl();
    const int surface_index = gap_index_;

    ParaxialTrace* tracer = new ParaxialTrace(opt_sys);

    double y0 = opt_sys->first_order_data()->ref_y0;
    double u0 = opt_sys->first_order_data()->ref_u0;

    u0 *= pupil_zone_;

    auto ax_ray = tracer->trace_paraxial_ray_from_object(y0, u0, ref_wvl);

    double u_prime = ax_ray->at(surface_index)->u_prime();
    double y       = ax_ray->at(surface_index)->y();

    double t = (height_ - y)/u_prime;

    delete tracer;

    // set value
    opt_sys->optical_assembly()->gap(gap_index_)->set_thi(t);

}

/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi All Rights Reserved.
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
** General Public License for more details.
**
** You should have received a copy of the GNU General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com
**             Date: October 9th, 2022
********************************************************************************/

#include "solve/overall_length_solve.h"
#include "system/optical_system.h"

using namespace geopter;

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

void OverallLengthSolve::set_parameters(int index, double param1, double param2, double param3)
{

}

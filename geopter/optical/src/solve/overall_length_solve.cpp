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

OverallLengthSolve::OverallLengthSolve()
{
    solve_type_ = SolveType::OverallLength;
    surface1_ = 0;
    surface2_ = 1;
    gap_index_ = 0;
    value_ = 0.0;
}

OverallLengthSolve::OverallLengthSolve(int gi, double value, int s1, int s2) : Solve()
{
    solve_type_ = SolveType::OverallLength;
    surface1_ = s1;
    surface2_ = s2;
    gap_index_ = gi;
    value_ = value;
}

bool OverallLengthSolve::Check(const OpticalSystem *opt_sys)
{
    if(surface1_ >= surface2_) return false;
    if(surface2_ > opt_sys->GetOpticalAssembly()->NumberOfSurfaces()-1) return false;
    if(gap_index_ > opt_sys->GetOpticalAssembly()->NumberOfGaps()-1) return false;

    return true;
}

void OverallLengthSolve::Apply(OpticalSystem *opt_sys)
{
    double cur_oal = opt_sys->GetOpticalAssembly()->OverallLength(surface1_, surface2_);
    double cur_thi = opt_sys->GetOpticalAssembly()->GetGap(gap_index_)->Thickness();
    double new_thi = (value_ - cur_oal) + cur_thi;

    opt_sys->GetOpticalAssembly()->GetGap(gap_index_)->SetThickness(new_thi);
}

void OverallLengthSolve::SetParameters(double param1, double param2, double param3, double /*param4*/)
{
    surface1_ = static_cast<int>(param1);
    surface2_ = static_cast<int>(param2);
    value_    = param3;
}

void OverallLengthSolve::GetParameters(double *param1, double *param2, double *param3, double *param4)
{
    *param1 = static_cast<double>(surface1_);
    *param2 = static_cast<double>(surface2_);
    param3 = nullptr;
    param4 = nullptr;
}

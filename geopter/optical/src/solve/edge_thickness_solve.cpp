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

#include "solve/edge_thickness_solve.h"
#include "system/optical_system.h"

using namespace geopter;

EdgeThicknessSolve::EdgeThicknessSolve()
{
    solve_type_ = SolveType::EdgeThickness;
}

EdgeThicknessSolve::EdgeThicknessSolve(int gap_index, double thickness, double radial_height)
{
    solve_type_ = SolveType::EdgeThickness;
    gap_index_ = gap_index;
    thickness_ = thickness;
    radial_height_ = radial_height;
}

bool EdgeThicknessSolve::Check(const OpticalSystem *opt_sys)
{
    if(gap_index_ < 0 || radial_height_ < 0.0 || thickness_ < 0.0) return false;

    if(gap_index_ > opt_sys->GetOpticalAssembly()->NumberOfGaps() -1) return false;

    return true;
}

void EdgeThicknessSolve::Apply(OpticalSystem* opt_sys)
{
    int s1 = gap_index_;
    int s2 = s1+1;
    double sag1 = opt_sys->GetOpticalAssembly()->GetSurface(s1)->Sag(0.0, radial_height_);
    double sag2 = opt_sys->GetOpticalAssembly()->GetSurface(s2)->Sag(0.0, radial_height_);

    double thi = thickness_ - ( -sag1 + sag2); //et = -sag1 + thi + sag2;

    opt_sys->GetOpticalAssembly()->GetGap(gap_index_)->SetThickness(thi);
}

void EdgeThicknessSolve::SetParameters(double param1, double param2, double /*param3*/, double /*param4*/)
{
    thickness_ = param1;
    radial_height_ = param2;
}

void EdgeThicknessSolve::GetParameters(double *param1, double *param2, double *param3, double *param4)
{
    *param1 = thickness_;
    *param2 = radial_height_;
    param3 = nullptr;
    param4 = nullptr;
}


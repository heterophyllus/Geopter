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

void EdgeThicknessSolve::set_parameters(int index, double param1, double param2, double param3)
{

}

/**
This file is part of Geopter.
Geopter is free software: you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

Geopter is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Geopter.
If not, see <https://www.gnu.org/licenses/>.
**/

#include "solve/pickup_solve.h"
#include "system/optical_system.h"

using namespace geopter;

PickupSolve::PickupSolve(Gap* gap)
{
    gap_ = gap;
    solve_type_ = Solve::SolveType::Pickup;
}

void PickupSolve::Apply(OpticalSystem *opt_sys)
{
    double from_thi = opt_sys->GetOpticalAssembly()->GetGap(from_gap_index_)->Thickness();
    double dst_thi = from_thi*scale_ + offset_;
    gap_->SetThickness(dst_thi);
}

void PickupSolve::SetParameters(double param1, double param2, double param3, double /*param4*/)
{
    from_gap_index_ = static_cast<int>(param1);
    scale_ = param2;
    offset_ = param3;
}

void PickupSolve::GetParameters(double *param1, double *param2, double *param3, double *param4)
{
    *param1 = static_cast<double>(from_gap_index_);
    *param2 = scale_;
    *param3 = offset_;
    param4 = nullptr;
}


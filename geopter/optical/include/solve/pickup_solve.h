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

#ifndef GEOPTER_PICKUP_SOLVE_H
#define GEOPTER_PICKUP_SOLVE_H

#include "solve/solve.h"

namespace geopter {

class Gap;

class PickupSolve : public Solve
{
    PickupSolve(Gap* gap);
    void Apply(OpticalSystem* opt_sys) override;
    void SetParameters(double param1, double param2, double param3=0, double param4=0) override;
    void GetParameters(double *param1, double *param2, double *param3, double *param4) override;

private:
    Gap* gap_;
    int from_gap_index_;
    double scale_;
    double offset_;
};

}

#endif //GEOPTER_PICKUP_SOLVE_H


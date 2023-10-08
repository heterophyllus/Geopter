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


#ifndef GEOPTER_FIXED_SOLVE_H
#define GEOPTER_FIXED_SOLVE_H

#include "solve/solve.h"

namespace geopter {

class FixedSolve : public Solve
{
public:
    FixedSolve(){
        solve_type_ = 0;
    }
    bool Check(const OpticalSystem* /*opt_sys*/) override{ return true;}
    void Apply(OpticalSystem* /*opt_sys*/) override{}
    int GetSolveType() const override { return 0; }
    void SetParameters(double /*param1*/, double /*param2*/, double /*param3=0.0*/, double /*param4=0.0*/) override{}
    void GetParameters(double *param1, double *param2, double *param3, double *param4) override{
        param1 = nullptr;
        param2 = nullptr;
        param3 = nullptr;
        param4 = nullptr;
    }

};

}

#endif

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

#ifndef GEOPTER_MARGINAL_RAY_HEIGHT_SOLVE_H
#define GEOPTER_MARGINAL_RAY_HEIGHT_SOLVE_H

#include "solve/solve.h"

namespace geopter {

class MarginalHeightSolve : public Solve
{
public:
    MarginalHeightSolve(int gi, double value, double zone);
    bool check(const OpticalSystem* opt_sys) override;
    void apply(OpticalSystem* opt_sys) override;
    void set_parameters(int index, double param1, double param2, double param3) override;

private:
    int gap_index_;
    double height_;
    double pupil_zone_;
};


}


#endif //GEOPTER_MARGINAL_RAY_HEIGHT_SOLVE_H


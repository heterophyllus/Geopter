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


#ifndef GEOPTER_EDGE_THICKNESS_SOLVE_H
#define GEOPTER_EDGE_THICKNESS_SOLVE_H

#include "solve/solve.h"

namespace geopter {


class EdgeThicknessSolve : public Solve
{
public:
    EdgeThicknessSolve();
    EdgeThicknessSolve(int gap_index, double thickness, double radial_height);
    bool Check(const OpticalSystem* opt_sys) override;
    void Apply(OpticalSystem* opt_sys) override;
    int GetSolveType() const override { return Solve::EdgeThickness; }
    std::string GetSolveTypeStr() const override { return "E"; }
    void SetParameters(double param1, double param2, double param3, double param4) override;
    void GetParameters(double *param1, double *param2, double *param3, double *param4) override;

private:
    int gap_index_;
    double thickness_;
    double radial_height_;
};

}


#endif //GEOPTER_EDGE_THICKNESS_SOLVE_H


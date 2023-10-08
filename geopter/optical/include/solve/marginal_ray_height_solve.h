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
    MarginalHeightSolve();
    MarginalHeightSolve(int gi, double value, double zone);
    bool Check(const OpticalSystem* opt_sys) override;
    void Apply(OpticalSystem* opt_sys) override;
    int GetSolveType() const override { return SolveType::MarginalHeight; }
    std::string GetSolveTypeStr() const override { return "M"; }
    void SetParameters(double param1, double param2, double param3, double param4) override;
    void GetParameters(double *param1=nullptr, double *param2=nullptr, double *param3=nullptr, double *param4=nullptr) override;

private:
    double height_;
    double pupil_zone_;
};


}


#endif //GEOPTER_MARGINAL_RAY_HEIGHT_SOLVE_H


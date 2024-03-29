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
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com
**             Date: October 9th, 2022
********************************************************************************/

#ifndef GEOPTER_OVERALL_LENGTH_SOLVE_H
#define GEOPTER_OVERALL_LENGTH_SOLVE_H

#include "solve/solve.h"

namespace geopter {

class OverallLengthSolve : public Solve
{
public:
    OverallLengthSolve();
    OverallLengthSolve(int gi, double value, int s1, int s2);
    bool Check(const OpticalSystem* opt_sys) override;
    void Apply(OpticalSystem* opt_sys) override;
    int GetSolveType() const override { return SolveType::OverallLength;}
    std::string GetSolveTypeStr() const override{ return "O";}
    void SetParameters(double param1, double param2, double param3, double param4) override;
    void GetParameters(double *param1=nullptr, double *param2=nullptr, double *param3=nullptr, double *param4=nullptr) override;

private:
    int surface1_;
    int surface2_;
    double value_;
};

}


#endif //GEOPTER_OVERALL_LENGTH_SOLVE_H

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
    OverallLengthSolve(int gi, double value, int s1, int s2);
    bool check(const OpticalSystem* opt_sys) override;
    void apply(OpticalSystem* opt_sys) override;
    void set_parameters(int index, double param1, double param2=0.0, double param3=0.0) override;

private:
    int surface1_;
    int surface2_;
    int gap_index_;
    double value_;
};

}


#endif //GEOPTER_OVERALL_LENGTH_SOLVE_H

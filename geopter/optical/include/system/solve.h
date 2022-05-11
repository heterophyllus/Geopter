/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi All Rights Reserved.
**
** This file is part of Geopter.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com
**             Date: May 4th, 2022
********************************************************************************/


#ifndef GEOPTER_SOLVE_H
#define GEOPTER_SOLVE_H

#include "assembly/surface.h"
#include "assembly/gap.h"

namespace geopter{

class OpticalSystem;

class Solve
{
public:

    enum SolveType
    {
        EdgeThickness,
        OverallLength,
        ParaxialImageDistance,
        ReductionRatio
    };

    Solve();
    virtual ~Solve();

    virtual void apply(OpticalSystem* opt_sys) = 0;

protected:
    double value_;
};


class EdgeThicknessSolve : public Solve
{
public:
    EdgeThicknessSolve(int gap_index, double value, double height);

    void apply(OpticalSystem* opt_sys) override;
private:
    int gap_index_;
    double height_;
};

class OverallLengthSolve : public Solve
{
public:
    OverallLengthSolve(int gi, double value, int s1, int s2);
    void apply(OpticalSystem* opt_sys) override;
private:
    int surface1_;
    int surface2_;
    int gap_index_;

};

class ParaxialImageSolve : public Solve
{
    // image space gap
public:
    ParaxialImageSolve();

    void apply(OpticalSystem* opt_sys) override;

};


} //namespace geopter

#endif //GEOPTER_SOLVE_H

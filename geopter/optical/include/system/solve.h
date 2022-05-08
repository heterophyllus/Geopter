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

enum SolveType
{
    EdgeThickness,
    OverallLength,
    ParaxialImageDistance,
    ReductionRatio
};

class Solve
{
public:
    Solve();
    virtual ~Solve();

    virtual void apply() = 0;

protected:
    double value_;
};


class EdgeThicknessSolve : public Solve
{
public:
    EdgeThicknessSolve();

    void apply() override; // center thickness for given edge thickness

private:
    Gap* gap_;
    Surface* srf_;
    Surface* srf_next_;
    double semi_diameter_;
};

class OverallLengthSolve : public Solve
{
public:
    OverallLengthSolve();

    void apply() override;

private:
    Gap* gap_;
    Surface* srf_start;
    Surface* srf_end_;
};

class ParaxialImageSolve : public Solve
{
public:
    ParaxialImageSolve();

    void apply() override;

    void set_gap(Gap* gap);

private:
    Gap* gap_; //sI-1
};


} //namespace geopter

#endif //GEOPTER_SOLVE_H

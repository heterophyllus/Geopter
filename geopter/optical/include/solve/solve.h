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
**             Date: May 4th, 2022
********************************************************************************/


#ifndef GEOPTER_SOLVE_H
#define GEOPTER_SOLVE_H

#include <string>

namespace geopter{

class OpticalSystem;

class Solve
{
public:

    enum SolveType
    {
        Fixed,
        EdgeThickness,
        OverallLength,
        MarginalHeight
    };

    Solve(){solve_type_ = -1;}
    virtual ~Solve(){};

    /** Check parameters */
    virtual bool Check(const OpticalSystem* /*opt_sys*/){ return true; }

    /** Apply solved value to the system */
    virtual void Apply(OpticalSystem* opt_sys) = 0;

    /** Returns solve type as integer. If -1, no valid solve is set */
    virtual int GetSolveType() const{return -1;}

    virtual std::string GetSolveTypeStr() const{return "";}

    virtual void SetParameters(double param1, double param2=0.0, double param3=0.0, double param4=0.0) = 0;
    virtual void GetParameters(double *param1=nullptr, double *param2=nullptr, double *param3=nullptr, double *param4=nullptr) = 0;

    void SetGapIndex(int gi) { gap_index_ = gi; }

protected:
    int gap_index_;
    int solve_type_;
};


} //namespace geopter

#endif //GEOPTER_SOLVE_H

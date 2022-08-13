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


namespace geopter{

class OpticalSystem;

class Solve
{
public:

    enum SolveType
    {
        EdgeThickness,
        OverallLength,
        MarginalHeight
    };

    Solve(){solve_type_ = -1;}
    virtual ~Solve(){};

    /** Check parameters */
    virtual bool check(const OpticalSystem* /*opt_sys*/){ return true; }

    /** Apply solved value to the system */
    virtual void apply(OpticalSystem* opt_sys) = 0;

    /** Returns solve type as integer. If -1, no valid solve is set */
    int solve_type() const {return solve_type_;}

protected:
    int solve_type_;
};


class EdgeThicknessSolve : public Solve
{
public:
    EdgeThicknessSolve(int gap_index, double value, double height);
    bool check(const OpticalSystem* opt_sys) override;
    void apply(OpticalSystem* opt_sys) override;

private:
    int gap_index_;
    double height_;
    double value_;
};

class OverallLengthSolve : public Solve
{
public:
    OverallLengthSolve(int gi, double value, int s1, int s2);
    bool check(const OpticalSystem* opt_sys) override;
    void apply(OpticalSystem* opt_sys) override;

private:
    int surface1_;
    int surface2_;
    int gap_index_;
    double value_;
};

class MarginalHeightSolve : public Solve
{

public:
    MarginalHeightSolve(int gi, double value, double zone);
    bool check(const OpticalSystem* opt_sys) override;
    void apply(OpticalSystem* opt_sys) override;

private:
    int gap_index_;
    double height_;
    double pupil_zone_;
};


} //namespace geopter

#endif //GEOPTER_SOLVE_H

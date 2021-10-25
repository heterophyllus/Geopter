/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
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
**             Date: May 16th, 2021                                                                                          
********************************************************************************/

//============================================================================
/// \file   paraxial_path.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#ifndef PARAXIALPATH_H
#define PARAXIALPATH_H

#include <vector>

namespace geopter {

struct ParaxialPathComponent
{
    ParaxialPathComponent() {
        c = 0.0;
        t = 0.0;
        n = 1.0;
    }

    ParaxialPathComponent(double cv, double thi, double ind){
        c = cv;
        t = thi;
        n = ind;
    }

    /** surface curvature */
    double c;

    /** gap thickness */
    double t;

    /** refractive index */
    double n;
};


class ParaxialPath
{
public:
    ParaxialPath();
    ~ParaxialPath();

    void prepend(ParaxialPathComponent par_path_comp);

    void append(double c, double t, double n);
    void append(ParaxialPathComponent par_path_comp);

    int size() const;

    ParaxialPathComponent at(int i) const;
    ParaxialPathComponent front() const;
    ParaxialPathComponent back() const;

    void clear();

private:
    std::vector<ParaxialPathComponent> par_path_comps_;
};

}

#endif // PARAXIALPATH_H

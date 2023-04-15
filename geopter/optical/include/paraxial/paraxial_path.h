/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
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
** You should have received a copy of the GNU Lesser General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi                                   
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com                          
**             Date: May 16th, 2021                                                                                          
********************************************************************************/


#ifndef PARAXIALPATH_H
#define PARAXIALPATH_H

#include <vector>

namespace geopter {

struct ParaxialPathComponent
{
    ParaxialPathComponent() {
        curvature        = 0.0;
        thickness        = 0.0;
        refractive_index = 1.0;
    }

    ParaxialPathComponent(double cv, double thi, double ind){
        curvature        = cv;
        thickness        = thi;
        refractive_index = ind;
    }

    /** surface curvature */
    double curvature;

    /** gap thickness */
    double thickness;

    /** refractive index */
    double refractive_index;
};


class ParaxialPath
{
public:
    ParaxialPath();
    ~ParaxialPath();

    void Prepend(ParaxialPathComponent par_path_comp);

    void Append(double c, double t, double n);
    void Append(ParaxialPathComponent par_path_comp);

    int Size() const;

    ParaxialPathComponent At(int i) const;
    ParaxialPathComponent Front() const;
    ParaxialPathComponent Back() const;

    void Clear();

private:
    std::vector<ParaxialPathComponent> prx_path_comps_;
};

}

#endif // PARAXIALPATH_H

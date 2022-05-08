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


#ifndef SEQUENTIALPATH_H
#define SEQUENTIALPATH_H

#include <vector>

#include "assembly/surface.h"

namespace geopter {


struct SequentialPathComponent
{
    SequentialPathComponent(){
        srf  = nullptr;
        d    = 0.0;
        rind = 1.0;
    }
    SequentialPathComponent(Surface* s, double thi, double n){
        srf  = s;
        d    = thi;
        rind = n;
    }
    ~SequentialPathComponent(){
        srf = nullptr;
    }

    Surface* srf;
    double d;
    double rind;
};


class SequentialPath
{
public:
    SequentialPath();
    ~SequentialPath();

    /** Returns number of path components */
    int size() const;

    /** Access to path component at the given index */
    SequentialPathComponent at(int i) const;

    /** Returns wavelength used to calculate refractive index */
    double wvl() const;

    /** Clear arrays */
    void clear();

    /** Append a new path component */
    void append(SequentialPathComponent seq_path_comp);

    /** Append a new path component */
    void append(Surface* s, double thi, double n);

    /** Set wavelength value used to calculate refractive index */
    void set_wvl(double wvl);

private:
    std::vector<SequentialPathComponent> seq_path_comps_;
    double wvl_;
    int array_size_;
};

}

#endif // SEQUENTIALPATH_H

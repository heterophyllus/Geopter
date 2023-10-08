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


#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include "spec/spectral_line.h"

namespace geopter {

/** Constant refractive index medium */
class Material
{
public:
    Material(){
        name_ = "";
        n_ = 1.0;
    }

    Material(double nd, std::string name){
        n_ = nd;
        name_ = name;
    }

    virtual ~Material(){}

    virtual std::string Name() const { return name_; }
    virtual void SetName(const std::string& name) { name_ = name; }

    /** Return refractive index at specified wavelength */
    virtual double RefractiveIndex(double /*wv_nm*/) const { return n_; }

    /** Returns Abbe number in d-lne */
    virtual double Abbe_d() const {
        double nd = RefractiveIndex(SpectralLine::d);
        double nF = RefractiveIndex(SpectralLine::F);
        double nC = RefractiveIndex(SpectralLine::C);

        return (nd - 1.0)/(nF - nC);
    }

protected:
    std::string name_;
    double n_;
};

} //namespace geopter

#endif // MATERIAL_H

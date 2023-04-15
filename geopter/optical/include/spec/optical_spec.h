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

#ifndef OPTICALSPEC_H
#define OPTICALSPEC_H

#include <memory>

#include "spec/pupil_spec.h"
#include "spec/field_spec.h"
#include "spec/wavelength_spec.h"

namespace geopter {

class OpticalSystem;

class OpticalSpec
{
public:
    OpticalSpec(OpticalSystem* opt_sys);
    ~OpticalSpec();

    PupilSpec* GetPupilSpec() { return pupil_.get(); }
    WavelengthSpec* GetWavelengthSpec() { return wavelength_spec_.get(); }
    FieldSpec* GetFieldSpec() { return field_spec_.get(); }

    void Clear();

    void CreateMinimumSpec();

    void update();

    void print(std::ostringstream& oss);

private:
    OpticalSystem* parent_;
    std::unique_ptr<WavelengthSpec> wavelength_spec_;
    std::unique_ptr<PupilSpec> pupil_;
    std::unique_ptr<FieldSpec> field_spec_;
};



} //namespace geopter

#endif // OPTICALSPEC_H

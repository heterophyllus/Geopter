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
#include "spec/wvl_spec.h"

namespace geopter {


class OpticalSpec
{
public:
    OpticalSpec();
    ~OpticalSpec();

    inline PupilSpec* pupil_spec();
    inline WvlSpec* spectral_region();
    inline FieldSpec* field_of_view();

    void clear();

    void create_minimum_spec();

    void print(std::ostringstream& oss);

private:
    std::unique_ptr<WvlSpec> spectral_region_;
    std::unique_ptr<PupilSpec> pupil_;
    std::unique_ptr<FieldSpec> field_of_view_;
};



PupilSpec* OpticalSpec::pupil_spec()
{
    return pupil_.get();
}

WvlSpec* OpticalSpec::spectral_region()
{
    return spectral_region_.get();
}

FieldSpec* OpticalSpec::field_of_view()
{
    return field_of_view_.get();
}


} //namespace geopter

#endif // OPTICALSPEC_H

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
**             Date: May 16th, 2021                                                                                          
********************************************************************************/

#ifndef OPTICALSYSTEM_H
#define OPTICALSYSTEM_H


#include <string>
#include <memory>
#include <sstream>
#include <vector>
#include <map>
#include <cassert>

#include "spec/optical_spec.h"
#include "assembly/optical_assembly.h"
#include "material/material_library.h"
#include "paraxial/first_order_data.h"

namespace geopter {

enum ReferenceRay{
    ChiefRay,
    MeridionalUpperRay,
    MeridionalLowerRay,
    SagittalUpperRay,
    SagittalLowerRay
};

class OpticalSystem
{
public:
    OpticalSystem();
    virtual ~OpticalSystem();

    void initialize();

    /** Returns the title of the system */
    std::string title() const { return title_; }

    /** Returns note attached to the system */
    std::string note() const { return note_; }

    /** Optical specifications; pupil, field, wavelength */
    OpticalSpec* optical_spec() const { return opt_spec_.get(); }

    /** sequential assembly of surfaces and gaps filled with material */
    OpticalAssembly* optical_assembly() const { return opt_assembly_.get(); }

    MaterialLibrary* material_lib() const { return material_lib_.get(); }

    FirstOrderData* first_order_data() const { return fod_.get(); }

    void load_file(const std::string& filepath);
    void save_to_file(const std::string& filepath);

    void set_title(std::string title) { title_ = title; }
    void set_note(std::string note) { note_ = note;}

    void set_vignetting_factors();

    void get_object_coord();

    void update_model();

    void clear();

    void print(std::ostringstream& oss);


protected:
    std::unique_ptr<OpticalAssembly> opt_assembly_;
    std::unique_ptr<OpticalSpec> opt_spec_;
    std::unique_ptr<FirstOrderData> fod_;
    std::unique_ptr<MaterialLibrary> material_lib_;

    std::string title_;
    std::string note_;
};



} //namespace geopter

#endif // OPTICALSYSTEM_H

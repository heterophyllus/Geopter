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
#include "system/fundamental_data.h"
#include "sequential/ray_at_surface.h"
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
    std::string title() const;

    /** Returns note attached to the system */
    std::string note() const;

    /** Optical specifications; pupil, field, wavelength */
    inline OpticalSpec* optical_spec() const;

    /** sequential assembly of surfaces and gaps filled with material */
    inline OpticalAssembly* optical_assembly() const;

    inline MaterialLibrary* material_lib() const;

    inline FirstOrderData* first_order_data() const;

    inline FundamentalData fundamental_data() const;

    void load_file(const std::string& filepath);
    void save_to_file(const std::string& filepath);

    void set_title(std::string title);
    void set_note(std::string text);

    void set_vignetting_factors();

    void get_object_coord();

    void update_model();

    void clear();

    void print(std::ostringstream& oss);


protected:
    void update_optical_spec();
    void update_paraxial_data();
    void update_semi_diameters();
    void update_fundamental_data();
    void update_solve();

    void transfer_to_exit_pupil(Surface* srf, const RayAtSurface* ray_seg, double exp_dist_parax);

    std::unique_ptr<OpticalAssembly> opt_assembly_;
    std::unique_ptr<OpticalSpec> opt_spec_;
    std::unique_ptr<FirstOrderData> fod_;
    std::unique_ptr<MaterialLibrary> material_lib_;

    std::string title_;
    std::string note_;

    // frequently used fundamental data
    FundamentalData fund_data_;
};


OpticalSpec* OpticalSystem::optical_spec() const
{
    return opt_spec_.get();
}

OpticalAssembly* OpticalSystem::optical_assembly() const
{
    return opt_assembly_.get();
}

FirstOrderData* OpticalSystem::first_order_data() const
{
    return fod_.get();
}


MaterialLibrary* OpticalSystem::material_lib() const
{
    return material_lib_.get();
}

FundamentalData OpticalSystem::fundamental_data() const
{
    return fund_data_;
}


} //namespace geopter

#endif // OPTICALSYSTEM_H

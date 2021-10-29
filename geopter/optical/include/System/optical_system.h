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

//============================================================================
/// \file   optical_system.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#ifndef OPTICALSYSTEM_H
#define OPTICALSYSTEM_H


#include <string>
#include <memory>
#include <sstream>
#include <vector>

#include "spec/optical_spec.h"
#include "assembly/optical_assembly.h"
#include "paraxial/first_order.h"
#include "paraxial/paraxial_ray.h"
#include "sequential/ray.h"
#include "material/material_library.h"

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
    ~OpticalSystem();

public:
    void set_title(std::string title);

    /** Returns the title of the system */
    std::string title() const;

    /** Returns note attached to the system */
    std::string note() const;
    void set_note(std::string text);

    /** 
     * @brief Optical specifications 
     * @note pupil, field, wavelength
     * */
    OpticalSpec* optical_spec() const;

    /** 
     * @brief sequential assembly of surfaces and gaps filled with material
     * */
    OpticalAssembly* optical_assembly() const;

    MaterialLibrary* material_lib() const;

    void create_minimum_system();


    std::shared_ptr<ParaxialRay> axial_ray(int wi) const;
    std::shared_ptr<ParaxialRay> principle_ray(int wi) const;
    FirstOrderData first_order_data() const;

    std::shared_ptr<Ray> reference_ray(int ri, int fi, int wi) const;

    void load_file(const std::string& filepath);
    void save_to_file(const std::string& filepath);

    void set_vignetting_factors();

    void update_model();

    void clear();

    void print(std::ostringstream& oss);

    void add_surface_and_gap(double r, double t, std::string mat_name);

private:

    void update_aim_pt();
    void update_reference_rays();
    void update_semi_diameters();
    void update_paraxial_data();
    void update_object_coords();

    std::unique_ptr<OpticalAssembly> opt_assembly_;
    std::unique_ptr<OpticalSpec> opt_spec_;
    std::unique_ptr<MaterialLibrary> material_lib_;

    // fundamental data
    int num_wvl_;
    int num_fld_;
    int ref_wvl_idx_;
    double ref_wvl_val_;

    // -----> Paraxial Data
    /** parallel to axis at s1 */
    std::shared_ptr<ParaxialRay> p_ray_;

    /** with slope at s1 */
    std::shared_ptr<ParaxialRay> q_ray_;

    /** paraxial axial rays computed with all wavelengths */
    std::vector<std::shared_ptr<ParaxialRay>> ax_rays_;

    /** paraxial principle rays computed with all wavelengths */
    std::vector<std::shared_ptr<ParaxialRay>> pr_rays_;

    /** first order data computed with all wavelengths */
    FirstOrderData fod_;

    // <----- Paraxial Data End


    // -----> Sequential Data
    /** reference rays */
    std::vector< std::shared_ptr<Ray> > chief_rays_;
    std::vector< std::shared_ptr<Ray> > meridional_upper_rays_;
    std::vector< std::shared_ptr<Ray> > meridional_lower_rays_;
    std::vector< std::shared_ptr<Ray> > sagittal_upper_rays_;
    std::vector< std::shared_ptr<Ray> > sagittal_lower_rays_;

    /** focus shifts */
    std::vector<double> x_focus_shifts_;
    std::vector<double> y_focus_shifts_;

    int to_ray_index(int fi, int wi) const {
        //int num_wvl = opt_spec_->spectral_region()->wvl_count();
        return num_wvl_ * (fi) + wi;
    }

    // <----- Sequential Data End

    std::string title_;
    std::string note_;
};

} //namespace geopter

#endif // OPTICALSYSTEM_H

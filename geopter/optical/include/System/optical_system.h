#ifndef OPTICALSYSTEM_H
#define OPTICALSYSTEM_H

#include <string>
#include <memory>
#include <sstream>
#include <vector>

#include "Spec/optical_spec.h"
#include "Assembly/optical_assembly.h"
#include "Paraxial/first_order.h"
#include "Paraxial/paraxial_ray.h"
#include "Sequential/ray.h"
#include "Material/material_library.h"

namespace geopter {

class OpticalSystem
{
public:
    OpticalSystem();
    ~OpticalSystem();

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

    void add_surface_and_gap(double r, double t, std::string mat_name);

    std::shared_ptr<ParaxialRay> axial_ray(int wi) const;
    std::shared_ptr<ParaxialRay> principle_ray(int wi) const;
    FirstOrderData first_order_data() const;

    std::shared_ptr<Ray> reference_ray(int ri, int fi, int wi) const;

    void update_vignetting_factors();

    void update_model();

    void clear();

    void print(std::ostringstream& oss);

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
    std::vector< std::shared_ptr<Ray> > ref_rays1_; // chief ray
    std::vector< std::shared_ptr<Ray> > ref_rays2_; // upper meridional marginal
    std::vector< std::shared_ptr<Ray> > ref_rays3_; // lower meridional marginal
    std::vector< std::shared_ptr<Ray> > ref_rays4_; // upper sagittal marginal
    std::vector< std::shared_ptr<Ray> > ref_rays5_; // lower sagittal marginal

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

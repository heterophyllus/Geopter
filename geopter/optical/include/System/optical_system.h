#ifndef OPTICALSYSTEM_H
#define OPTICALSYSTEM_H

#include <string>
#include <memory>

namespace geopter {

class OpticalAssembly;
class OpticalSpec;
class MaterialLibrary;
class ParaxialModel;
class SequentialModel;
class ElementModel;

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

    /**
     * @brief paraxial data container
     * @note paraxial rays, first order data
     */
    ParaxialModel* parax_data() const;

    /**
     * @brief optical data computed by real ray tracing
     * @note reference rays, x/y focus shifts
     * @return SequentialModel* 
     */
    SequentialModel* sequential_data() const;

    MaterialLibrary* material_lib() const;

    ElementModel* elem_model() const;

    void create_minimum_system();

    void update_vignetting_factors();

    void update_model();

    void clear();

private:
    std::unique_ptr<OpticalAssembly> opt_assembly_;
    std::unique_ptr<OpticalSpec> opt_spec_;
    std::unique_ptr<ParaxialModel> parax_data_;
    std::unique_ptr<SequentialModel> seq_data_;
    std::unique_ptr<ElementModel> elem_model_;
    std::unique_ptr<MaterialLibrary> material_lib_;

    std::string title_;
    std::string note_;
};

} //namespace geopter

#endif // OPTICALSYSTEM_H

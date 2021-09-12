#include <iostream>

#include "System/optical_system.h"

#include "Assembly/optical_assembly.h"

#include "Paraxial/paraxial_model.h"

#include "Spec/optical_spec.h"
#include "Spec/pupil_spec.h"
#include "Spec/field_spec.h"
#include "Spec/wvl_spec.h"

#include "Sequential/sequential_model.h"

#include "Element/element_model.h"

#include "Material/material_library.h"


using namespace geopter;

OpticalSystem::OpticalSystem() : 
    title_(""),
    note_("")
{
    opt_spec_     = std::make_unique<OpticalSpec>(this);
    opt_assembly_ = std::make_unique<OpticalAssembly>(this);
    parax_data_   = std::make_unique<ParaxialModel>(this);
    seq_data_     = std::make_unique<SequentialModel>(this);
    material_lib_ = std::make_unique<MaterialLibrary>();
    elem_model_   = std::make_unique<ElementModel>(this);
}

OpticalSystem::~OpticalSystem()
{
    opt_assembly_.reset();
    opt_spec_.reset();
    parax_data_.reset();
    seq_data_.reset();
    material_lib_.reset();
    elem_model_.reset();
}

std::string OpticalSystem::title() const
{
    return title_;
}

void OpticalSystem::set_title(std::string title)
{
    title_ = title;
}

std::string OpticalSystem::note() const
{
    return note_;
}

void OpticalSystem::set_note(std::string note)
{
    note_ = note;
}

OpticalSpec* OpticalSystem::optical_spec() const
{
    return opt_spec_.get();
}

OpticalAssembly* OpticalSystem::optical_assembly() const
{
    return opt_assembly_.get();
}

ParaxialModel* OpticalSystem::parax_data() const
{
    return parax_data_.get();
}

SequentialModel* OpticalSystem::sequential_data() const
{
    return seq_data_.get();
}

MaterialLibrary* OpticalSystem::material_lib() const
{
    return material_lib_.get();
}

ElementModel* OpticalSystem::elem_model() const
{
    return elem_model_.get();
}


void OpticalSystem::create_minimum_system()
{
    opt_assembly_->create_minimun_assembly();

    opt_spec_->field_of_view()->clear();
    opt_spec_->field_of_view()->add(0.0, 0.0, 1.0, rgb_black);

    opt_spec_->spectral_region()->clear();
    opt_spec_->spectral_region()->add(SpectralLine::d, 1.0, rgb_black);

    opt_spec_->pupil_spec()->set_value(PupilType::EPD);
    opt_spec_->pupil_spec()->set_value(10);
}



void OpticalSystem::update_model()
{
    opt_assembly_->update_model();
    parax_data_->update_model();
    seq_data_->update_model();

    opt_assembly_->set_semi_diameters();
    elem_model_->update_model();
}



void OpticalSystem::clear()
{
    title_ = "";
    note_ = "";
    opt_assembly_->clear();
    opt_spec_->clear();
}

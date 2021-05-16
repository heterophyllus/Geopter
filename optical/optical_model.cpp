#include "optical_model.h"

#include "optical_spec.h"
#include "sequential_model.h"
#include "paraxial_model.h"
#include "element_model.h"

#include "database.h"

#include "pupil_spec.h"
#include "field_spec.h"
#include "wvl_spec.h"

#include "surface.h"
#include "gap.h"
#include "air.h"

using namespace geopter;

OpticalModel::OpticalModel(std::shared_ptr<Database> database) :
    database_(database)
{
    if(database_ == nullptr){
        database_ = std::make_shared<Database>();
    }

    optical_spec_ = std::make_unique<OpticalSpec>(this);
    seq_model_    = std::make_unique<SequentialModel>(this);
    parax_model_  = std::make_unique<ParaxialModel>(this);
    elem_model_   = std::make_unique<ElementModel>(this);
}

OpticalModel::~OpticalModel()
{
    database_.reset();
    optical_spec_.reset();
    seq_model_.reset();
    parax_model_.reset();
    elem_model_.reset();
}

void OpticalModel::set_name(std::string name)
{
    name_ = name;
}
std::string OpticalModel::name() const
{
    return name_;
}

std::shared_ptr<Database> OpticalModel::database() const
{
    return database_;
}

Database* OpticalModel::get_database() const
{
    return database_.get();
}

void OpticalModel::set_database(std::shared_ptr<Database> d)
{
    database_ = d;
}

OpticalSpec* OpticalModel::optical_spec() const
{
    return optical_spec_.get();
}

ParaxialModel* OpticalModel::paraxial_model() const
{
    return parax_model_.get();
}

SequentialModel* OpticalModel::seq_model() const
{
    return seq_model_.get();
}

ElementModel* OpticalModel::elem_model() const
{
    return elem_model_.get();
}

void OpticalModel::set_default_model()
{
    //seq_model_->clear();
    seq_model_->initialize_arrays();
    seq_model_->gap(0)->set_thi(1.0e+6);
    seq_model_->add_surface(0.0, 0.0, std::make_shared<Air>());
    seq_model_->set_stop();

    optical_spec_->pupil_spec()->set_pupil_type(PupilType::EPD);
    optical_spec_->pupil_spec()->set_value(2.0);

    optical_spec_->field_of_view()->clear();
    optical_spec_->field_of_view()->add_field(0.0, 0.0, 1.0, rgb_black);

    optical_spec_->spectral_region()->clear();
    optical_spec_->spectral_region()->add(SpectralLine::d, 1.0, rgb_black);

    //update_model();
}

void OpticalModel::update_model()
{
    seq_model_->update_model();
    parax_model_->update_model();
    optical_spec_->update_model();
    //elem_model_->update_model();
}

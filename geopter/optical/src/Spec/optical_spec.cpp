#include "Spec/optical_spec.h"


using namespace geopter;

OpticalSpec::OpticalSpec()
{
    spectral_region_ = std::make_unique<WvlSpec>();
    pupil_           = std::make_unique<PupilSpec>();
    field_of_view_   = std::make_unique<FieldSpec>();
}

OpticalSpec::~OpticalSpec()
{
    spectral_region_.reset();
    pupil_.reset();
    field_of_view_.reset();
}


void OpticalSpec::create_minimum_spec()
{
    field_of_view_->clear();
    field_of_view_->add(0.0, 0.0, 1.0, rgb_black);

    spectral_region_->clear();
    spectral_region_->add(SpectralLine::d, 1.0, rgb_black);

    pupil_->set_value(PupilType::EPD);
    pupil_->set_value(10);
}

void OpticalSpec::clear()
{
    spectral_region_->clear();
    field_of_view_->clear();
}


void OpticalSpec::print(std::ostringstream &oss)
{
    oss << "Pupil Specs..." << std::endl;
    pupil_->print(oss);

    oss << "Wavelengths..." << std::endl;
    spectral_region_->print(oss);

    oss << "Fields..." << std::endl;
    field_of_view_->print(oss);

}

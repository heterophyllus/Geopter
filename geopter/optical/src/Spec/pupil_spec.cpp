#include <iostream>
#include <iomanip>

#include "Spec/pupil_spec.h"

using namespace geopter;

PupilSpec::PupilSpec(int pupil_type, double value) :
    pupil_type_(pupil_type),
    value_(value)
{
    create_default_pupil_rays();
    pupil_rays_ = default_pupil_rays_;
}

PupilSpec::~PupilSpec()
{

}

void PupilSpec::set_pupil_type(int i)
{
    switch (i) {
    case PupilType::EPD:
    case PupilType::FNO:
    case PupilType::NA:
    case PupilType::NAO:
        pupil_type_ = i;
        break;
    default:
        throw("Unknown pupil type");
    }
}

int PupilSpec::pupil_type() const
{
    return pupil_type_;
}

std::vector<PupilCrd> PupilSpec::pupil_rays() const
{
    return pupil_rays_;
}

void PupilSpec::set_value(double val)
{
    value_ = val;
}

double PupilSpec::value() const
{
    return value_;
}

void PupilSpec::update_model()
{
    pupil_rays_ = default_pupil_rays_;
}

void PupilSpec::create_default_pupil_rays()
{
    default_pupil_rays_.clear();
    default_pupil_rays_.push_back(PupilCrd({ 0.0,  0.0}));
    default_pupil_rays_.push_back(PupilCrd({ 1.0,  0.0}));
    default_pupil_rays_.push_back(PupilCrd({-1.0,  0.0}));
    default_pupil_rays_.push_back(PupilCrd({ 0.0,  1.0}));
    default_pupil_rays_.push_back(PupilCrd({ 0.0, -1.0}));
}

void PupilSpec::print(std::ostringstream &oss)
{
    constexpr int label_w = 15;

    oss << std::setw(label_w) << std::left << "Type: ";
    switch (pupil_type_) {
    case PupilType::EPD:
        oss << "Entrance Pupil Diameter";
        break;
    case PupilType::FNO:
        oss << "F-Number";
        break;
    case PupilType::NA:
        oss << "Image Space NA(Numerical Aperture)";
        break;
    case PupilType::NAO:
        oss << "Object Space NA(Numerical Aperture)";
        break;
    }
    oss << std::endl;


    oss << std::setw(label_w) << std::left << "Value: ";
    oss << value_ << std::endl;
    oss << std::endl;

}

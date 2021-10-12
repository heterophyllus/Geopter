#include <iostream>
#include <iomanip>

#include "Spec/pupil_spec.h"

using namespace geopter;

PupilSpec::PupilSpec() :
    pupil_type_(PupilType::EPD),
    value_(10.0)
{

}


PupilSpec::PupilSpec(int pupil_type, double value) :
    pupil_type_(pupil_type),
    value_(value)
{

}

PupilSpec::~PupilSpec()
{

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

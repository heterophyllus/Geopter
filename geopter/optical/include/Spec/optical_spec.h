#ifndef OPTICALSPEC_H
#define OPTICALSPEC_H

#include <memory>

#include "Spec/pupil_spec.h"
#include "Spec/field_spec.h"
#include "Spec/wvl_spec.h"

namespace geopter {


class OpticalSpec
{
public:
    OpticalSpec();
    ~OpticalSpec();

    inline PupilSpec* pupil_spec();
    inline WvlSpec* spectral_region();
    inline FieldSpec* field_of_view();

    void clear();

    void create_minimum_spec();

    void print(std::ostringstream& oss);

private:
    std::unique_ptr<WvlSpec> spectral_region_;
    std::unique_ptr<PupilSpec> pupil_;
    std::unique_ptr<FieldSpec> field_of_view_;

    int stop_;
};



PupilSpec* OpticalSpec::pupil_spec()
{
    return pupil_.get();
}

WvlSpec* OpticalSpec::spectral_region()
{
    return spectral_region_.get();
}

FieldSpec* OpticalSpec::field_of_view()
{
    return field_of_view_.get();
}


} //namespace geopter

#endif // OPTICALSPEC_H

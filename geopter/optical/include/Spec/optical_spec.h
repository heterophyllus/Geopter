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

    PupilSpec* pupil_spec();
    WvlSpec* spectral_region();
    FieldSpec* field_of_view();

    void update_model();

    void clear();

    void create_minimum_spec();

    void print(std::ostringstream& oss);

private:
    std::unique_ptr<WvlSpec> spectral_region_;
    std::unique_ptr<PupilSpec> pupil_;
    std::unique_ptr<FieldSpec> field_of_view_;

    int stop_;
};

} //namespace geopter

#endif // OPTICALSPEC_H

#ifndef OPTICALSPEC_H
#define OPTICALSPEC_H

#include <memory>

namespace geopter {

class OpticalSystem;
class PupilSpec;
class WvlSpec;
class FieldSpec;

class OpticalSpec
{
public:
    OpticalSpec(OpticalSystem* sys);
    ~OpticalSpec();

    PupilSpec* pupil_spec();
    WvlSpec* spectral_region();
    FieldSpec* field_of_view();

    void update_model();

    void clear();

    void print(std::ostringstream& oss);

private:
    OpticalSystem* opt_sys_;
    std::unique_ptr<WvlSpec> spectral_region_;
    std::unique_ptr<PupilSpec> pupil_;
    std::unique_ptr<FieldSpec> field_of_view_;

    int stop_;

    bool do_aiming_;
};

} //namespace geopter

#endif // OPTICALSPEC_H

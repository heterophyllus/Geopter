#ifndef OPTICALSPEC_H
#define OPTICALSPEC_H


#include "Eigen/Core"

namespace geopter {

class OpticalModel;
class PupilSpec;
class WvlSpec;
class FieldSpec;
class Field;

class OpticalSpec
{
public:
    OpticalSpec(OpticalModel* opt_model);
    ~OpticalSpec();

    PupilSpec* pupil_spec();
    WvlSpec* spectral_region();
    FieldSpec* field_of_view();

    Eigen::Vector3d obj_coord(const Field& fld);

    void update_model();

    void print(std::ostringstream& oss);

private:
    OpticalModel* opt_model_;
    std::unique_ptr<WvlSpec> spectral_region_;
    std::unique_ptr<PupilSpec> pupil_;
    std::unique_ptr<FieldSpec> field_of_view_;

    int stop_;
    double wvl_;

    bool do_aiming_;
};

} //namespace geopter

#endif // OPTICALSPEC_H

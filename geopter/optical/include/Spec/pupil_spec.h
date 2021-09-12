#ifndef PUPILSPEC_H
#define PUPILSPEC_H


#include <vector>

#include "Eigen/Core"
using PupilCrd = Eigen::Vector2d;

namespace geopter {

enum PupilType
{
    EPD,
    FNO,
    NAO,
    NA
};


class PupilSpec
{

public:
    PupilSpec(int pupil_type=PupilType::EPD, double value=1.0);
    ~PupilSpec();

    void update_model();

    void set_pupil_type(int i);

    int pupil_type() const;

    void set_value(double val);

    /** Returns pupil value */
    double value() const;

    std::vector<PupilCrd> pupil_rays() const;

    void print(std::ostringstream& oss);

private:
    int pupil_type_;
    double value_;

    /** list of relative pupil coordinates for pupil limiting rays*/
    std::vector<PupilCrd> pupil_rays_;

    std::vector<PupilCrd> default_pupil_rays_;

    void create_default_pupil_rays();
};

} //namespace geopter

#endif // PUPILSPEC_H

#ifndef PUPILSPEC_H
#define PUPILSPEC_H

#include <cassert>
#include <vector>

#include "Eigen/Core"

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
    PupilSpec();
    PupilSpec(int pupil_type, double value);
    ~PupilSpec();

    inline int pupil_type() const;

    inline double value() const;

    inline void set_pupil_type(int i);

    inline void set_value(double val);

    void print(std::ostringstream& oss);

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

private:
    int pupil_type_;
    double value_;
};



int PupilSpec::pupil_type() const
{
    return pupil_type_;
}

double PupilSpec::value() const
{
    return value_;
}

void PupilSpec::set_pupil_type(int i)
{
    assert( 0 <= i && i < 4);
    pupil_type_ = i;
}

void PupilSpec::set_value(double val)
{
    value_ = val;
}


} //namespace geopter

#endif // PUPILSPEC_H

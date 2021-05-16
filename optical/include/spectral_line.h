#ifndef SPECTRALLINE_H
#define SPECTRALLINE_H

#include <string>

namespace geopter {


/** Constant values for Fraunhofer spectral line */
class SpectralLine
{
public:

    /** get wavelength value from spectral line name */
    static double wavelength(const std::string& spectralname);

    static const double t;
    static const double s;
    static const double r;
    static const double C;
    static const double C_;
    static const double D;
    static const double d;
    static const double e;
    static const double F;
    static const double F_;
    static const double g;
    static const double h;
    static const double i;
};

} //namespace geopter

#endif // SPECTRALLINE_H

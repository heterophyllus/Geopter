#ifndef PARAXIALPATH_H
#define PARAXIALPATH_H

#include <vector>

namespace geopter {


struct ParaxialPathComponent
{
    /** surface index */
    int   si;

    /** surface curvature */
    double c;

    /** gap thickness */
    double t;

    /** refractive index */
    double n;

    double z_dir;
};

using ParaxialPath = std::vector<ParaxialPathComponent>;

}

#endif // PARAXIALPATH_H

#ifndef PARAXIALRAY_H
#define PARAXIALRAY_H

#include <vector>
#include <iostream>

namespace geopter {


struct ParaxialRayAtSurface
{
    double ht  = 0.0;
    double slp = 0.0;
    double aoi = 0.0;
};

/** This struct is used to define initial input for compute_first_order, etc */
struct ParaxialRayHtAndSlp
{
    double ht;
    double slp;
};


class ParaxialRay
{
public:
    ParaxialRay();
    ~ParaxialRay();

    void append(ParaxialRayAtSurface par_comp);
    void append(double ht, double slp, double aoi= 0);

    void clear();

    /** Return number of components, usually equal to number of surfaces. */
    int size();

    ParaxialRayAtSurface at(int i);

    /** Return the last surface data */
    ParaxialRayAtSurface back();

    /** Write ray data to standard output */
    void print();

    /** Write property data to stream */
    void print(std::ostringstream& oss);

private:
    std::vector<ParaxialRayAtSurface> par_ray_at_srfs_;
};

} //namespace geopter

#endif // PARAXIALRAY_H

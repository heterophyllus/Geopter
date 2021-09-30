#ifndef PARAXIALRAY_H
#define PARAXIALRAY_H

#include <vector>
#include <string>
#include <sstream>

#include "paraxial_ray_at_surface.h"

namespace geopter {


/** Paraxial ray */
class ParaxialRay
{
public:
    ParaxialRay();
    ~ParaxialRay();

    void prepend(std::shared_ptr<ParaxialRayAtSurface> par_ray_at_srf);
    void prepend(double y, double u_prime, double i= 0, double n_prime = 1.0);
    
    void append(std::shared_ptr<ParaxialRayAtSurface> par_ray_at_srf);
    void append(double y, double u_prime, double i= 0, double n_prime = 1.0);

    void clear();

    std::string name() const;
    void set_name(std::string name);

    /** Return number of components, usually equal to number of surfaces. */
    int size() const;

    /** Access to data at the given index */
    std::shared_ptr<ParaxialRayAtSurface> at(int i) const;

    /** Returns the data at the beginning */
    std::shared_ptr<ParaxialRayAtSurface> front() const;

    /** Return the data at the last surface */
    std::shared_ptr<ParaxialRayAtSurface> back() const;

    /** Access to the data at the surface just before the image */
    std::shared_ptr<ParaxialRayAtSurface> at_before_image() const;

    /** Write ray data to standard output */
    void print() const;

    /** Write property data to stream */
    void print(std::ostringstream& oss) const;

private:
    std::vector< std::shared_ptr<ParaxialRayAtSurface> > par_ray_at_srfs_;
    std::string name_;
};

} //namespace geopter

#endif // PARAXIALRAY_H

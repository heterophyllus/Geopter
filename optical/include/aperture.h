#ifndef APERTURE_H
#define APERTURE_H

#include <string>

namespace geopter{


struct ApertureDimension
{
    /** horizontal length from the origin */
    double x;

    /** vertical length from the origin */
    double y;
};


/** Apertures on surfaces used to define and limit the light beam passing through a lens system */
class Aperture
{
public:
    Aperture();
    virtual ~Aperture();

    virtual ApertureDimension dimension() const;
    virtual void set_dimension(double x, double y);
    virtual double max_dimension() const =0;
    virtual bool point_inside(double x, double y) const =0;
    virtual std::string shape_name() const =0;

protected:
    ApertureDimension dimension_;
    double x_offset_;
    double y_offset_;
    double rotation_;
};

} //namespace

#endif // APERTURE_H

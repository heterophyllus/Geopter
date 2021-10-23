#ifndef APERTURE_H
#define APERTURE_H

#include <string>
#include "Eigen/Core"

namespace geopter{


/** Apertures on surfaces used to define and limit the light beam passing through a lens system */
class Aperture
{
public:
    enum Shape
    {
        Circular,
        Rectangular
    };

    Aperture();
    virtual ~Aperture();

    virtual double x_dimension() const;
    virtual double y_dimension() const;
    virtual Eigen::Vector2d dimension() const;
    virtual double max_dimension() const =0;
    virtual void set_dimension(double x, double y);

    virtual bool point_inside(double x, double y) const =0;
    virtual std::string shape_name() const =0;

protected:
    double x_dimension_;
    double y_dimension_;
    double x_offset_;
    double y_offset_;
    double rotation_;
};

} //namespace

#endif // APERTURE_H

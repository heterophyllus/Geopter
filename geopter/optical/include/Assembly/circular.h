#ifndef CIRCULAR_H
#define CIRCULAR_H

#include "aperture.h"

namespace geopter {


class Circular : public Aperture
{
public:
    Circular(double r=1.0);
    ~Circular();

    void set_radius(double r);

    std::string shape_name() const override;
    double max_dimension() const override;
    void set_dimension(double x, double y) override;
    bool point_inside(double x, double y) const override;

private:
    double radius_;
};

} //namespace


#endif // CIRCULAR_H

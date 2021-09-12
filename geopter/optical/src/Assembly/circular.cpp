
#define _USE_MATH_DEFINES
#include <math.h>

#include "Assembly/circular.h"

using namespace geopter;

Circular::Circular(double r) : Aperture()
{
    radius_ = r;
    x_dimension_ = radius_;
    y_dimension_ = radius_;
}

Circular::~Circular()
{

}

void Circular::set_radius(double r)
{
    radius_ = r;
}

std::string Circular::shape_name() const
{
    return "Circular";
}

double Circular::max_dimension() const
{
    return radius_;
}

void Circular::set_dimension(double x, double y)
{
    radius_ = sqrt(x*x + y*y);
}

bool Circular::point_inside(double x, double y) const
{
    if( (x*x + y*y) < radius_*radius_ ){
        return true;
    }else{
        return false;
    }
}

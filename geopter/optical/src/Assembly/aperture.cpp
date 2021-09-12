#include "Assembly/aperture.h"

using namespace geopter;

Aperture::Aperture() :
    x_dimension_(0.0),
    y_dimension_(0.0),
    x_offset_(0.0),
    y_offset_(0.0),
    rotation_(0.0)
{
    
}

Aperture::~Aperture()
{

}

double Aperture::x_dimension() const
{
    return x_dimension_;
}

double Aperture::y_dimension() const
{
    return y_dimension_;
}

Eigen::Vector2d Aperture::dimension() const
{
    Eigen::Vector2d dim({x_dimension_, y_dimension_});
    return dim;
}

void Aperture::set_dimension(double x, double y)
{
    x_dimension_ = x;
    y_dimension_ = y;
}

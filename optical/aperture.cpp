#include "aperture.h"

using namespace geopter;

Aperture::Aperture() :
    x_offset_(0.0),
    y_offset_(0.0),
    rotation_(0.0)
{
    dimension_.x = 0.0;
    dimension_.y = 0.0;
}

Aperture::~Aperture()
{

}

ApertureDimension Aperture::dimension() const
{
    return dimension_;
}

void Aperture::set_dimension(double x, double y)
{
    dimension_.x = x;
    dimension_.y = y;
}

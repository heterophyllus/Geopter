#ifndef GEOPTER_NONE_APERTURE_H
#define GEOPTER_NONE_APERTURE_H

#include "aperture/aperture.h"

namespace geopter {

class NoneAperture : public Aperture
{
public:
    NoneAperture(){
        aperture_name_ = "None";
    }

    double MaxDimension() const override{
        return 0.0;
    }

    bool PointInside(double /*x*/, double /*y*/) const override{
        return true;
    }

};


} // namespace


#endif //GEOPTER_NONE_APERTURE_H

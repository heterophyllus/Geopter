#ifndef RAY_BUNDLE_H
#define RAY_BUNDLE_H

#include "ray.h"

namespace geopter{

enum RayPattern{
    Grid,
    Hexapolar,
    Random
};

template<enum RayPattern P> //pattern
class RayBundle
{
public:
    RayBundle(/* args */);
    ~RayBundle();

    void allocate();

private:
    std::vector< std::shared_ptr<Ray> > rays_;
};


}



#endif



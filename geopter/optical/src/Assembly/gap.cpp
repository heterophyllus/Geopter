#include "Assembly/gap.h"

#include "Material/air.h"

using namespace geopter;

Gap::Gap()
{
    thi_ = 0.0;
    material_ = std::make_shared<Air>();
}

Gap::Gap(double t, std::shared_ptr<Material> m)
{
    thi_ = t;

    if(m){
        material_ = m;
    }else{
        material_ = std::make_shared<Air>();
    }
    
}


Gap::~Gap()
{
    material_.reset();
}



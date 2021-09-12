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
    material_ = nullptr;
}

double Gap::thi() const
{
    return thi_;
}

void Gap::set_thi(double t)
{
    thi_ = t;
}

void Gap::set_material(std::shared_ptr<Material> m)
{
    material_ = m;
}

Material* Gap::material() const
{
    return material_.get();
}

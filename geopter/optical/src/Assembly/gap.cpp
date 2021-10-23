#include "Assembly/gap.h"


using namespace geopter;


Gap::Gap()
{
    thi_ = 0.0;
    material_ = MaterialLibrary::air().get();
}

Gap::Gap(double t, Material* m)
{
    thi_ = t;

    if(m){
        material_ = m;
    }else{
        material_ = MaterialLibrary::air().get();
    }
    
}


Gap::~Gap()
{
    material_ = nullptr;
}

void Gap::set_material(Material *m)
{
    if(m){
        material_ = m;
    }else{
        material_ = MaterialLibrary::air().get();
    }
}

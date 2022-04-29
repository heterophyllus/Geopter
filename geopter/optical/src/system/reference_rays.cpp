
#include "system/reference_rays.h"
using namespace geopter;

ReferenceRays::ReferenceRays()
{

}

ReferenceRays::~ReferenceRays()
{
    for(auto i : chief_rays_){
        i.clear();
    }
    chief_rays_.clear();
}

void ReferenceRays::clear()
{
    for(auto i : chief_rays_){
        i.clear();
    }
    chief_rays_.clear();
}

void ReferenceRays::allocate(int num_fld, int num_wvl)
{
    this->clear();


}

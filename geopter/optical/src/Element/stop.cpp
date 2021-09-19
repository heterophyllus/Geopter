#include "Element/stop.h"

using namespace geopter;

Stop::Stop(Surface* s) :
    s_(s)
{
    element_type_ = "Stop";
}

Stop::~Stop()
{
    s_ = nullptr;
}

Surface* Stop::reference_surface() const
{
    return s_;;
}

void Stop::draw(Renderer* renderer)
{

}

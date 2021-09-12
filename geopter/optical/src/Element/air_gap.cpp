#include "Element/air_gap.h"

using namespace geopter;

AirGap::AirGap(Gap* g) :
    gap_(g)
{
    element_type_ = "AirGap";
}

AirGap::~AirGap()
{
    gap_ = nullptr;
}


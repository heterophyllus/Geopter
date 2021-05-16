#include "gap.h"
#include "air.h"

using namespace geopter;

Gap::Gap(double t, std::shared_ptr<Medium> med)
{
    thi_ = t;
    medium_ = med;
}


Gap::~Gap()
{
    medium_ = nullptr;
}

double Gap::thi() const
{
    return thi_;
}

void Gap::set_thi(double t)
{
    thi_ = t;
}

void Gap::set_medium(std::shared_ptr<Medium> med)
{
    medium_ = med;
}

Medium* Gap::medium() const
{
    return medium_.get();
}

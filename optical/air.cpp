#include "air.h"

using namespace geopter;

Air::Air()
{
    name_ = "AIR";
    display_name_ = "AIR";
    n_ = 1.0;
}

Air::~Air()
{

}

double Air::rindex(double wv_nm) const
{
    return 1.0;
}

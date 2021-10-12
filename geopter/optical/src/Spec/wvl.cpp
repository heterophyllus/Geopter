#include "Spec/wvl.h"

using namespace geopter;

Wvl::Wvl(double wl, double wt, Rgb render_color) :
    value_(wl),
    weight_(wt),
    render_color_(render_color)
{

}

Wvl::~Wvl()
{

}


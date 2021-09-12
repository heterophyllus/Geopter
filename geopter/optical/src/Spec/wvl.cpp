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

void Wvl::set_value(double val_nm)
{
    value_ = val_nm;
}

double Wvl::value() const
{
    return value_;
}

void Wvl::set_weight(double wt)
{
    weight_ = wt;
}

double Wvl::weight() const
{
    return weight_;
}

void Wvl::set_render_color(Rgb color)
{
    render_color_ = color;
}

Rgb Wvl::render_color() const
{
    return render_color_;
}

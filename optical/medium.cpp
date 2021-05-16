#include "medium.h"

using namespace geopter;

Medium::Medium(double nd, std::string name) :
    n_(nd),
    name_(name)
{
    display_name_ = name_;
}

Medium::~Medium()
{

}

std::string Medium::name() const
{
    return name_;
}

void Medium::set_name(std::string name)
{
    name_ = name;
}

std::string Medium::display_name() const
{
    return display_name_;
}

void Medium::set_display_name(std::string disp_name)
{
    display_name_ = disp_name;
}

double Medium::rindex(double wv_nm) const
{
    return n_;
}

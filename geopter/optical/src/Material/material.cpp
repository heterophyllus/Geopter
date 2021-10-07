#include "Material/material.h"

#include "Spec/spectral_line.h"

using namespace geopter;

Material::Material(double nd, std::string name) :
    n_(nd),
    name_(name)
{
    
}

Material::~Material()
{

}

std::string Material::name() const
{
    return name_;
}

void Material::set_name(std::string name)
{
    name_ = name;
}

double Material::rindex(double wv_nm) const
{
    return n_;
}

double Material::abbe_d() const
{
    double nd = rindex(SpectralLine::d);
    double nF = rindex(SpectralLine::F);
    double nC = rindex(SpectralLine::C);

    return (nd - 1.0)/(nF - nC);
}

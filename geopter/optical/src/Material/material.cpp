#include "Material/material.h"

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

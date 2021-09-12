#include "Element/element.h"

#include "Renderer/renderer.h"

using namespace geopter;

Element::Element()
{
    label_ = "";
    element_type_ = "";
}

Element::~Element()
{

}

std::string Element::label() const
{
    return label_;
}

std::string Element::element_type() const
{
    return element_type_;
}

void Element::draw(Renderer* renderer)
{

}

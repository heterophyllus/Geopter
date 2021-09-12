#include "Element/dummy_interface.h"

#include "Assembly/surface.h"
#include "Renderer/renderer.h"

using namespace geopter;

DummyInterface::DummyInterface(Surface* s) :
    s_(s)
{
    element_type_ = "DummyInterface";
}

DummyInterface::~DummyInterface()
{
    s_ = nullptr;
}

Surface* DummyInterface::surface() const
{
    return s_;
}

void DummyInterface::draw(Renderer* renderer)
{

}
#include "Element/lens.h"


using namespace geopter;

Lens::Lens(Surface* s1, Surface* s2, Gap* g) :
    s1_(s1),
    s2_(s2),
    gap_(g)
{
    element_type_ = "Lens";
}

Lens::~Lens()
{
    s1_ = nullptr;
    s2_ = nullptr;
    gap_ = nullptr;
}

Surface* Lens::surface1() const
{
    return s1_;
}

Surface* Lens::surface2() const
{
    return s2_;;
}

void Lens::draw(Renderer* renderer)
{

}

void Lens::draw_edge(Renderer* renderer)
{

}

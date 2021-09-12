#ifndef LENS_H
#define LENS_H

#include "element.h"

namespace geopter {

class Surface;
class Gap;

class Lens : public Element
{
public:
    Lens(Surface* s1, Surface* s2, Gap* g);
    ~Lens();

    Surface* surface1() const;
    Surface* surface2() const;
    
    void draw(Renderer* renderer) override;

private:
    void draw_edge(Renderer* renderer);

    Surface* s1_;
    Surface* s2_;
    Gap* gap_;

    double max_edge_extent_;
    double min_edge_extent_;
};

} // namespace

#endif
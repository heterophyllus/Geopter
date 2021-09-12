#ifndef STOP_H
#define STOP_H

#include "element.h"

namespace geopter{

class Surface;

class Stop : public Element
{
public:
    Stop(Surface* s);
    ~Stop();

    Surface* reference_surface() const;
    void draw(Renderer* renderer) override;

private:
    Surface* s_;
};

} // namespace

#endif

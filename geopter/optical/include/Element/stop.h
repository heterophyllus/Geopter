#ifndef STOP_H
#define STOP_H

#include "element.h"

#include "Assembly/surface.h"

namespace geopter{


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

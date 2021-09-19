#ifndef DUMMY_INTERFACE_H
#define DUMMY_INTERFACE_H

#include "Element/element.h"
#include "Assembly/surface.h"

namespace geopter {


class DummyInterface : public Element
{
public:
    DummyInterface(Surface* s);
    ~DummyInterface();

    Surface* surface() const;
    void draw(Renderer* renderer) override;

private:
    Surface* s_;
};

} // namespace

#endif

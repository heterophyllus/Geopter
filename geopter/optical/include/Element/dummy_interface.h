#ifndef DUMMY_INTERFACE_H
#define DUMMY_INTERFACE_H

#include "element.h"

namespace geopter {

class Surface;

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
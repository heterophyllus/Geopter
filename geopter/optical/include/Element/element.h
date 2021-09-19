#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>

#include "Renderer/renderer.h"

namespace geopter {

/** Base class to describe optical element */
class Element
{
public:
    Element();
    virtual ~Element();

    virtual std::string label() const;

    virtual std::string element_type() const;

    virtual void draw(Renderer* renderer);

    //virtual double weight() const;
    //virtual double volume() const;

protected:
    std::string label_;
    std::string element_type_;
};

}

#endif

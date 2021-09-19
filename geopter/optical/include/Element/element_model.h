#ifndef ELEMENT_MODEL_H
#define ELEMENT_MODEL_H

#include <vector>
#include <memory>

#include "System/optical_system.h"
#include "Element/element.h"

namespace geopter{


class ElementModel
{
public:
    ElementModel(OpticalSystem* sys);
    ~ElementModel();

    /** Returns pointer to element at the given index */
    Element* element(int i) const;

    /** Returns number of elements */
    int element_count() const;

    /** Create elements list */
    void create();

    /** Clear elements list */
    void clear();

    
    void update_model();

private:
    OpticalSystem* opt_sys_;
    std::vector< std::unique_ptr<Element> > elements_;
};

}

#endif

#ifndef ELEMENTMODEL_H
#define ELEMENTMODEL_H

#include <vector>
#include "transform.h"

namespace geopter {

class OpticalModel;
class SequentialModel;
class Element;
class Surface;
class Gap;

/** Maintain the element based representation of the optical model */
class ElementModel
{
public:
    ElementModel(OpticalModel* opt_model);
    ~ElementModel();

    /** get element at the index */
    Element* element(int i);

    int element_count();

    void update_model();

    void list_elements();

    /** Sort elements in order of reference interfaces in seq_model */
    void sequence_elements();

    /** generate an element list from a sequential model */
    void elements_from_sequence();

    void add_element(std::shared_ptr<Element> e);

    void add_dummy_interface_at_image(Tfrm tfrm);

    void relabel_airgaps();

private:
    void process_airgap(int i, Gap* g, Surface* s, Tfrm tfrm, int num_ele, bool add_ele=true);

protected:
    OpticalModel* opt_model_;
    std::vector< std::shared_ptr<Element> > elements_;
};

} //namespace geopter


#endif // ELEMENTMODEL_H

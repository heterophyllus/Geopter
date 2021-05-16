#ifndef ELEMENT_H
#define ELEMENT_H


#include <string>

#include "transform.h"

namespace geopter {


class Interface;
class Surface;
class Gap;
class SequentialModel;

class Element
{
public:
    Element();
    virtual ~Element();

    std::string label() const;
    void set_label(const std::string label);

    virtual void set_transform(Tfrm tfrm);

    virtual double sd() const;
    virtual void set_sd(double semidiam);

    virtual void sync_to_update(const SequentialModel& seq_model);

    virtual double update_size();

    virtual Interface* reference_interface();
    virtual int reference_idx();

protected:
    std::string label_;
    Tfrm tfrm_;
    std::string medium_name_;
    double sd_;
    Interface* ref_ifc_;
    int idx_;
};


class Lens : public Element
{
public:
    Lens(Surface* s1, Surface* s2, Gap* g, Tfrm tfrm, int idx=0, int idx2=1, double sd=1.0, std::string label="Lens");
    ~Lens();

    void set_sd(double semidiam) override;
    Interface* reference_interface() override;
    int reference_idx() override;
    void sync_to_update(const SequentialModel& seq_model) override;
    double update_size() override;

    Surface* surface1();
    Surface* surface2();

protected:
    Surface* s1_;
    Surface* s2_;
    int s1_indx_;
    int s2_indx_;
    Gap* gap_;

    double max_edge_extent_;
    double min_edge_extent_;
};



class DummyInterface : public Element
{
public:
    DummyInterface(Interface* ifc, int idx=0);
    DummyInterface(Interface* ifc, int idx, double sd, Tfrm tfrm, std::string label="DummyInterface");

    void sync_to_update(const SequentialModel& seq_model) override;
};


class AirGap : public Element
{
public:
    AirGap(Gap* g, Interface* ref_ifc, int idx, Tfrm tfrm, std::string label="AirGap");
    ~AirGap();

private:
    Gap* gap_;

};

}

#endif // ELEMENT_H

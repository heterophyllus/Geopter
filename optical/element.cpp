#include "element.h"
#include "sequential_model.h"
#include "interface.h"
#include "surface.h"
#include "gap.h"
#include "medium.h"

using namespace geopter;

/*********************************************************************************************
 * Element class
 * ********************************************************************************************/

Element::Element()
{

}

Element::~Element()
{
    ref_ifc_ = nullptr;
}


std::string Element::label() const
{
    return label_;
}

void Element::set_label(const std::string label)
{
    label_ = label;
}

void Element::set_transform(Tfrm tfrm)
{
    tfrm_ = tfrm;
}

double Element::sd() const
{
    return sd_;
}

void Element::set_sd(double semidiam)
{
    sd_ = semidiam;
}


Interface* Element::reference_interface()
{
    return ref_ifc_;
}

int Element::reference_idx()
{
    return idx_;
}

double Element::update_size()
{
    sd_ = ref_ifc_->max_aperture();
    return sd_;
}

void Element::sync_to_update(const SequentialModel& seq_model)
{

}

/***************************************************************************************************************
 * Lens class
 * *************************************************************************************************************/
Lens::Lens(Surface* s1, Surface* s2, Gap* g, Tfrm tfrm, int idx, int idx2, double sd, std::string label)
{
    tfrm_ = tfrm;
    s1_ = s1;
    s1_indx_ = idx;
    s2_ = s2;
    s2_indx_ = idx2;

    label_ = label;

    gap_ = g;
    medium_name_ = gap_->medium()->name();

    sd_ = sd;
}

Lens::~Lens()
{
    ref_ifc_ = nullptr;
}

void Lens::set_sd(double semidiam)
{
    sd_ = semidiam;
    //_edge_extent = {-semidiam, semidiam};
    min_edge_extent_ = -semidiam;
    max_edge_extent_ = semidiam;
}

Interface* Lens::reference_interface()
{
    return s1_;
}

int Lens::reference_idx()
{
    return s1_indx_;
}

void Lens::sync_to_update(const SequentialModel& seq_model)
{
    /*
     *  # when updating, we want to use the stored object instances to get the
        # current indices into the interface list (e.g. to handle insertion and
        # deletion of interfaces)
      */
    //s1_indx_ = seq_model.get_surface_index(s1_);
    //s2_indx_ = seq_model.get_surface_index(s2_);
}

double Lens::update_size()
{
    /*
    auto od1 = _s1->get_y_aperture_extent();
    auto od2 = _s2->get_y_aperture_extent();

    _edge_extent.max_aperture = std::max(od1.max_aperture, od2.max_aperture);
    _edge_extent.min_aperture = std::min(od1.min_aperture, od2.min_aperture);
    */

    double s1_max_ap, s1_min_ap;
    double s2_max_ap, s2_min_ap;

    s1_->get_y_aperture_extent(s1_min_ap, s1_max_ap);
    s2_->get_y_aperture_extent(s2_min_ap, s2_max_ap);

    min_edge_extent_ = std::min(s1_min_ap, s2_min_ap);
    max_edge_extent_ = std::max(s1_max_ap, s2_max_ap);

    sd_ = std::max(s1_->max_aperture(), s2_->max_aperture());

    return sd_;
}

Surface* Lens::surface1()
{
    return s1_;
}

Surface* Lens::surface2()
{
    return s2_;
}

/********************************************************************************************************
 * DummyInterface class
 * *******************************************************************************************************/
DummyInterface::DummyInterface(Interface* ifc, int idx)
{
    ref_ifc_ = ifc;
    idx_ = idx;

    tfrm_.rotation = Eigen::Matrix3d::Identity(3,3);
    tfrm_.transfer = Eigen::Vector3d::Zero(3);

    medium_name_ = "Interface";

    sd_ = ifc->max_aperture();

    label_ = "DummyInterface";

}

DummyInterface::DummyInterface(Interface* ifc, int idx, double sd, Tfrm tfrm, std::string label)
{
    ref_ifc_ = ifc;
    idx_ = idx;
    tfrm_ = tfrm;
    label_ = label;

    medium_name_ = "Interface";

    sd_ = sd;
}

void DummyInterface::sync_to_update(const SequentialModel& seq_model)
{
    //self.idx = seq_model.ifcs.index(self.ref_ifc)
}


/***********************************************************************************************************************
 * AirGap class
 * *********************************************************************************************************************/

AirGap::AirGap(Gap* g, Interface* ref_ifc, int idx, Tfrm tfrm, std::string label)
{
    gap_ = g;
    ref_ifc_ = ref_ifc;
    medium_name_ = gap_->medium()->name();
    idx_ = idx;
    label_ = label;
}

AirGap::~AirGap()
{

}

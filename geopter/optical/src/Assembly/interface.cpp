#include "Assembly/interface.h"

#include "Assembly/circular.h"
#include "Assembly/spherical.h"

using namespace geopter;

Interface::Interface() :
    interact_mode_("transmit"),
    semi_diameter_(0.0)
{
    label_ = "";
    lcl_tfrm_.rotation = Eigen::Matrix3d::Identity(3,3);
    lcl_tfrm_.transfer = Eigen::Vector3d::Zero(3);

    gbl_tfrm_.rotation = Eigen::Matrix3d::Identity(3,3);
    gbl_tfrm_.transfer = Eigen::Vector3d::Zero(3);

    profile_ = std::make_unique<Spherical>(0.0);
    
    decenter_ = nullptr;
}

Interface::~Interface()
{
    profile_.reset();

    if(edge_aperture_){
        edge_aperture_.reset();
    }
    if(clear_aperture_){
        clear_aperture_.reset();
    }
    //decenter_.reset();
}

std::string Interface::label() const
{
    return label_;
}

void Interface::set_label(std::string lbl)
{
    label_ = lbl;
}

void Interface::set_local_transform(Transformation tfrm)
{
    lcl_tfrm_ = tfrm;
}

void Interface::set_global_transform(Transformation tfrm)
{
    gbl_tfrm_ = tfrm;
}

Transformation Interface::local_transform() const
{
    return lcl_tfrm_;
}

Transformation Interface::global_transform() const
{
    return gbl_tfrm_;
}

std::string Interface::interact_mode() const
{
    return interact_mode_;
}

DecenterData* Interface::decenter() const
{
    return decenter_.get();
}

void Interface::update()
{
    if(decenter_){
        decenter_->update();
    }
}


double Interface::max_aperture() const
{
    if(clear_aperture_){
        return clear_aperture_->max_dimension();
    }else{
        return semi_diameter_;
    }

}


double Interface::semi_diameter() const
{
    return semi_diameter_;
}

void Interface::set_semi_diameter(double sd)
{
    semi_diameter_ = sd;
}

/*
void Interface::set_delta_n(double dn)
{
    delta_n_ = dn;
}
*/

/*
void Interface::set_optical_power(double pwr, double n_before, double n_after)
{

}
*/

void Interface::set_edge_aperture(std::unique_ptr<Aperture> ea)
{
    if(edge_aperture_){
        edge_aperture_.reset();
    }
    edge_aperture_ = std::move(ea);
}

void Interface::set_clear_aperture(std::unique_ptr<Aperture> ca)
{
    if(clear_aperture_){
        clear_aperture_ .reset();
    }
    clear_aperture_ = std::move(ca);
}

Aperture* Interface::clear_aperture() const
{
    return clear_aperture_.get();
}

Aperture* Interface::edge_aperture() const
{
    return edge_aperture_.get();
}

std::string Interface::aperture_shape() const
{
    if(clear_aperture_){
        return clear_aperture_->shape_name();
    }else{
        return "None";
    }
}

void Interface::remove_clear_aperture()
{
    if(clear_aperture_){
        clear_aperture_.reset();
    }
}

bool Interface::point_inside(double x, double y) const
{
    if(clear_aperture_) {
        return clear_aperture_->point_inside(x, y);
    }
    return true;
}

bool Interface::point_inside(Eigen::Vector2d pt) const
{
    if(clear_aperture_) {
        return clear_aperture_->point_inside(pt(0), pt(1));
    }
    return true;
}

void Interface::set_profile(std::unique_ptr<SurfaceProfile> prf)
{
    profile_.reset();
    profile_ = std::move(prf);
}

SurfaceProfile* Interface::profile() const
{
    return profile_.get();
}

#include "Assembly/interface.h"

#include "Assembly/circular.h"


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



void Interface::set_label(std::string lbl)
{
    label_ = lbl;
}

void Interface::set_local_transform(const Transformation& tfrm)
{
    lcl_tfrm_ = tfrm;
}

void Interface::set_global_transform(const Transformation& tfrm)
{
    gbl_tfrm_ = tfrm;
}


double Interface::max_aperture() const
{
    if(clear_aperture_){
        return clear_aperture_->max_dimension();
    }else{
        return semi_diameter_;
    }

}


void Interface::set_semi_diameter(double sd)
{
    semi_diameter_ = sd;
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

bool Interface::point_inside(const Eigen::Vector2d& pt) const
{
    if(clear_aperture_) {
        return clear_aperture_->point_inside(pt(0), pt(1));
    }
    return true;
}

void Interface::update()
{
    if(decenter_){
        decenter_->update();
    }
}


#include <iostream>
#include <sstream>

#include "Assembly/surface.h"

#include "Assembly/surface_profile.h"
#include "Assembly/spherical.h"
#include "Assembly/aperture.h"

#include "Sequential/trace_error.h"



using namespace geopter;

Surface::Surface(std::string lbl) : Interface()
{
    label_ = lbl;
    profile_ = std::make_unique<Spherical>();
    semi_diameter_ = 0.0;
}

Surface::~Surface()
{
    profile_.reset();
}

void Surface::update()
{
    profile_->update();
}

/*
double Surface::optical_power() const
{
    return delta_n_*profile_->cv();
}
*/

/*
void Surface::set_optical_power(double pwr, double n_before, double n_after)
{
    delta_n_ = n_after - n_before;
    optical_power_ = pwr;
}
*/

/*
void Surface::set_optical_power(double pwr)
{
    if(delta_n_ != 0.0){
        profile_->set_cv(pwr/delta_n_);
    }
    else{
        profile_->set_cv(0.0);
    }
    optical_power_ = pwr;
}
*/

/*
void Surface::get_y_aperture_extent(double &max_aperture, double &min_aperture)
{
    max_aperture = 1.0e+10;
    min_aperture = -1.0e+10;

    if(edge_aperture_){
        ApertureDimension plus, minus;
        if(plus.y < max_aperture){
            max_aperture = plus.y;
        }
        if(minus.y > min_aperture){
            min_aperture = minus.y;
        }
    }else if(clear_aperture_){
        ApertureDimension plus, minus;
        if(plus.y < max_aperture){
            max_aperture = plus.y;
        }
        if(minus.y > min_aperture){
            min_aperture = minus.y;
        }
    }else{
        min_aperture = -this->max_aperture();
        max_aperture =  this->max_aperture();
    }

}
*/

void Surface::intersect(Eigen::Vector3d &pt, double &s, Eigen::Vector3d p0, Eigen::Vector3d d, double eps, double z_dir)
{
    profile_->intersect(pt,s,p0,d,eps,z_dir);
}

Eigen::Vector3d Surface::normal(Eigen::Vector3d p)
{
    return profile_->normal(p);
}

void Surface::print()
{
    std::ostringstream oss;
    print(oss);
    std::cout << oss.str() << std::endl;
}

void Surface::print(std::ostringstream& oss)
{
    oss << label_ << ": ";
    oss << profile_->cv() << ", ";
    oss << max_aperture();
}

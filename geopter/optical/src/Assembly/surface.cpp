#include <iostream>
#include <sstream>

#include "Assembly/surface.h"
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


void Surface::intersect(Eigen::Vector3d &pt, double &s, const Eigen::Vector3d& p0, const Eigen::Vector3d& d, double eps, double z_dir)
{
    profile_->intersect(pt,s,p0,d,eps,z_dir);
}

Eigen::Vector3d Surface::normal(Eigen::Vector3d p)
{
    try{
        return profile_->normal(p);
    }catch(TraceMissedSurfaceError &e){
        std::cout << "TraceMissedSurfaceError : Surface::normal()" << std::endl;
        throw e;
    }
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

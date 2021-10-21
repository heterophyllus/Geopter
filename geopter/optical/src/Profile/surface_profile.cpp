#include "Profile/surface_profile.h"

using namespace geopter;

SurfaceProfile::SurfaceProfile()
{
    name_ = "None";
    cv_ = 0.0;
}


SurfaceProfile::~SurfaceProfile()
{

}

std::string SurfaceProfile::name() const
{
    return name_;
}

void SurfaceProfile::set_cv(double c)
{
    cv_ = c;
}

double SurfaceProfile::cv() const
{
    return cv_;
}

void SurfaceProfile::set_radius(double r)
{
    if(std::isnan(r)){
        return;
    }else if(std::isinf(r)){
        cv_ = 0.0;
    }else if(fabs(r) < std::numeric_limits<double>::epsilon()){
        cv_ = 0.0;
    }else{
        cv_ = 1.0/r;
    }
}

double SurfaceProfile::radius() const
{
    double r = 0.0;
    try {
        r = 1.0/cv_;
    }  catch (...) {
        r = 0.0;
    }
    return r;
}

double SurfaceProfile::f(const Eigen::Vector3d& p) const
{
    return 0.0;
}

Eigen::Vector3d SurfaceProfile::df(const Eigen::Vector3d& p) const
{
    return Eigen::Vector3d::Zero(3);
}

Eigen::Vector3d SurfaceProfile::normal(const Eigen::Vector3d& p) const
{
    return df(p).normalized();
}

double SurfaceProfile::sag(double x, double y) const
{
    return 0.0;
}

double SurfaceProfile::deriv_1st(double h) const
{
    return 0.0;
}

double SurfaceProfile::deriv_2nd(double h) const
{
    return 0.0;
}

void SurfaceProfile::intersect(Eigen::Vector3d& pt, double& s, const Eigen::Vector3d& p0, const Eigen::Vector3d& d, double eps, double z_dir)
{
    intersect_spencer(pt, s, p0, d, eps, z_dir);
}


void SurfaceProfile::intersect_spencer(Eigen::Vector3d& pt, double& s, const Eigen::Vector3d& p0, const Eigen::Vector3d& d, double eps, double z_dir)
{
    Eigen::Vector3d p = p0;
    double s1 = -f(p)/d.dot(df(p));
    double s2;
    double delta = fabs(s1);
    constexpr int max_itr = 50;
    int iter = 0;

    while(delta > eps && iter < max_itr)
    {
        p = p0 + s1*d;
        s2 = s1 - f(p)/d.dot(df(p));
        delta = fabs(s2-s1);
        s1 = s2;
        iter++;
    }

    pt = p;
    s = s1;
}

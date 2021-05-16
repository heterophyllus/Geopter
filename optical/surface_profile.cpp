#include "surface_profile.h"

using namespace geopter;

SurfaceProfile::SurfaceProfile()
{
    name_ = "None";
    cv_ = 0.0;
}

SurfaceProfile::~SurfaceProfile()
{

}

void SurfaceProfile::update()
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
    }else if(std::isnan(r)){
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

double SurfaceProfile::f(Eigen::Vector3d p) const
{
    return 0.0;
}

Eigen::Vector3d SurfaceProfile::df(Eigen::Vector3d p) const
{
    return Eigen::Vector3d::Zero(3);
}

Eigen::Vector3d SurfaceProfile::normal(Eigen::Vector3d p) const
{
    return df(p).normalized();
}

double SurfaceProfile::sag(double x, double y) const
{
    return 0.0;
}

/*
IntersectPointAndDistance SurfaceProfile::intersect(Vector3d p0, Vector3d d, double eps, int z_dir)
{
    return intersect_spencer(p0,d,eps,z_dir);
}
*/

void SurfaceProfile::intersect(Eigen::Vector3d& pt, double& s, Eigen::Vector3d p0, Eigen::Vector3d d, double eps, int z_dir)
{
    intersect_spencer(pt, s, p0, d, eps, z_dir);
}

/*
IntersectPointAndDistance SurfaceProfile::intersect_spencer(Vector3d p0, Vector3d d, double eps, int z_dir)
{
    Vector3d p = p0;
    double s1 = -f(p)/d.dot(df(p));
    double delta = abs(s1);

    int iter = 0;

    while(delta > eps && iter < 50)
    {
        p = p0 + s1*d;
        auto s2 = s1 - f(p)/d.dot(df(p));
        delta = abs(s2-s1);
        s1 = s2;
        iter++;
    }

    IntersectPointAndDistance sd;
    sd.intersect_point = p;
    sd.distance = s1;

    return sd;
}
*/

void SurfaceProfile::intersect_spencer(Eigen::Vector3d& pt, double& s, Eigen::Vector3d p0, Eigen::Vector3d d, double eps, int z_dir)
{
    Eigen::Vector3d p = p0;
    double s1 = -f(p)/d.dot(df(p));
    double delta = abs(s1);

    int iter = 0;

    while(delta > eps && iter < 50)
    {
        p = p0 + s1*d;
        auto s2 = s1 - f(p)/d.dot(df(p));
        delta = abs(s2-s1);
        s1 = s2;
        iter++;
    }

    /*
    IntersectPointAndDistance sd;
    sd.intersect_point = p;
    sd.distance = s1;

    return sd;
    */

    pt = p;
    s = s1;
}

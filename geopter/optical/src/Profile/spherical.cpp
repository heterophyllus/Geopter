#include <iostream>

#include "Profile/spherical.h"

#include "Sequential/trace_error.h"

using namespace geopter;

Spherical::Spherical(double c)
{
    name_ = "SPH";
    cv_ = c;
}

Spherical::~Spherical()
{

}

double Spherical::f(const Eigen::Vector3d& p) const
{
    return p(2) - 0.5*cv_*p.dot(p);
}

Eigen::Vector3d Spherical::df(const Eigen::Vector3d& p) const
{
    Eigen::Vector3d df;
    df(0) = -cv_*p(0);
    df(1) = -cv_*p(1);
    df(2) = 1.0 - cv_*p(2);

    return df;
}


double Spherical::sag(double x, double y) const
{
    if(fabs(cv_) < std::numeric_limits<double>::epsilon())
    {
        return 0.0;
    }
    else
    {
        double r = 1/cv_;
        double adj2 = r*r - x*x - y*y;

        if(adj2 < 0.0){
            return NAN;
        }

        double adj = sqrt(r*r - x*x - y*y);

        return r*(1-fabs(adj/r));
    }

}


void Spherical::intersect(Eigen::Vector3d &pt, double &s, const Eigen::Vector3d& p0, const Eigen::Vector3d& d, double eps, double z_dir)
{
    Eigen::Vector3d p = p0;
    double ax2 = cv_;
    double cx2 = cv_*(p.dot(p)) - 2*p(2);
    double b = cv_*(d.dot(p)) - d(2);

    double inside_sqrt = b*b - ax2*cx2;

    if(inside_sqrt < 0.0){
        throw TraceMissedSurfaceError();
    }
    else{
        double s1 = cx2/(z_dir*sqrt(b*b - ax2*cx2) -b );
        Eigen::Vector3d p1 = p + s1*d;

        pt = p1;
        s = s1;
    }
}

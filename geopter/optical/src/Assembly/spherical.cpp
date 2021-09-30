#include "Assembly/spherical.h"

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
    if(cv_ != 0.0)
    {
        double r = 1/cv_;
        double adj;
        try {
            adj = sqrt(r*r - x*x - y*y);
        }  catch (...) {
            throw "TraceMissedSurfaceError";
        }
        return r*(1-abs(adj/r));
    }
    else
    {
        return 0.0;
    }

}


void Spherical::intersect(Eigen::Vector3d &pt, double &s, const Eigen::Vector3d& p0, const Eigen::Vector3d& d, double eps, int z_dir)
{
    Eigen::Vector3d p = p0;
    double ax2 = cv_;
    double cx2 = cv_*(p.dot(p)) - 2*p(2);
    double b = cv_*(d.dot(p)) - d(2);

    double s1;
    try {
        s1 = cx2/((double)z_dir*sqrt(b*b - ax2*cx2) -b );
    }  catch (...) {
        // raise TraceMissedSurfaceError
        TraceMissedSurfaceError e;
        throw(e);
    }

    Eigen::Vector3d p1 = p + s1*d;

    /*
    IntersectPointAndDistance pd;
    pd.intersect_point = p1;
    pd.distance = s;

    return pd;
    */

    pt = p1;
    s = s1;
}

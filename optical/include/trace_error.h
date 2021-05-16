#ifndef TRACEERROR_H
#define TRACEERROR_H

#include "ray.h"
#include "Eigen/Core"

namespace geopter {

class Surface;

class TraceError:public std::exception
{
public:
    TraceError();
    virtual ~TraceError();
};


class TraceTIRError : public TraceError
{
public:
    TraceTIRError(Eigen::Vector3d inc_dir, Eigen::Vector3d normal, double prev_indx, double follow_indx);
    ~TraceTIRError();

    Eigen::Vector3d inc_dir_;
    Eigen::Vector3d normal_;
    double prev_indx_;
    double follow_indx_;
    std::vector<RayAtSurface> ray_;
    int surf_;
};

class TraceMissedSurfaceError : public TraceError
{
public:
    TraceMissedSurfaceError(Surface* ifc=nullptr);
    ~TraceMissedSurfaceError();

    Surface* ifc_;
    int surf_;
    std::vector<RayAtSurface> ray_;
};


class TraceBlockedByApertureError : public TraceError
{
public:
    TraceBlockedByApertureError(Surface* ifc=nullptr);
    ~TraceBlockedByApertureError();

};

} //namespace geopter

#endif // TRACEERROR_H

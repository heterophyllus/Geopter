#ifndef TRACEERROR_H
#define TRACEERROR_H

#include "ray.h"

namespace geopter {

class Surface;

class TraceError : public std::exception
{
public:
    TraceError();
    virtual ~TraceError();

    virtual std::string cause_str() const;
    std::shared_ptr<Ray> ray() const;
    int surface_index() const;

    void set_ray(std::shared_ptr<Ray> ray);
    void set_surface(Surface* s);
    void set_surface_index(int i);

protected:
    std::string cause_str_;
    std::shared_ptr<Ray> ray_;
    Surface* caused_surface_;
    int surface_index_;
};


class TraceTIRError : public TraceError
{
public:
    TraceTIRError();
};

class TraceMissedSurfaceError : public TraceError
{
public:
    TraceMissedSurfaceError();
};


class TraceBlockedByApertureError : public TraceError
{
public:
    TraceBlockedByApertureError();
};

class TraceRayAimingFailedError : public TraceError
{
public:
    TraceRayAimingFailedError();
};

} //namespace geopter

#endif // TRACEERROR_H

#include "Sequential/trace_error.h"

using namespace geopter;

TraceError::TraceError() :
    caused_surface_(nullptr),
    surface_index_(0)
{
    cause_str_ = "Unnown trace error";
}

TraceError::~TraceError()
{
    caused_surface_ = nullptr;
}

std::string TraceError::cause_str() const
{
    return cause_str_;
}

std::shared_ptr<Ray> TraceError::ray() const
{
    return ray_;
}

int TraceError::surface_index() const
{
    return surface_index_;
}

void TraceError::set_ray(std::shared_ptr<Ray> ray)
{
    ray_ = ray;
}

void TraceError::set_surface(Surface *s)
{
    caused_surface_ = s;
}

void TraceError::set_surface_index(int i)
{
    surface_index_ = i;
}


//**********************************************************
TraceTIRError::TraceTIRError()
{
    cause_str_ = "Total reflection";
}

//**********************************************************
TraceMissedSurfaceError::TraceMissedSurfaceError()
{
    cause_str_ = "Ray missed surface";
}

//**********************************************************
TraceBlockedByApertureError::TraceBlockedByApertureError()
{
    cause_str_ = "Ray blocked by aperture";
}

//**********************************************************
TraceRayAimingFailedError::TraceRayAimingFailedError()
{
    cause_str_ = "Ray aiming failed";
}

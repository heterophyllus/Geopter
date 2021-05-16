#include "trace_error.h"

using namespace geopter;

TraceError::TraceError()
{

}

TraceError::~TraceError()
{

}


TraceTIRError::TraceTIRError(Eigen::Vector3d inc_dir, Eigen::Vector3d normal, double prev_indx, double follow_indx) :
    inc_dir_(inc_dir),
    normal_(normal),
    prev_indx_(prev_indx),
    follow_indx_(follow_indx)
{

}

TraceTIRError::~TraceTIRError()
{
    ray_.clear();
}

TraceMissedSurfaceError::TraceMissedSurfaceError(Surface* ifc)
{
    ifc_ = ifc;
}

TraceMissedSurfaceError::~TraceMissedSurfaceError()
{
    ray_.clear();
}


TraceBlockedByApertureError::TraceBlockedByApertureError(Surface* ifc)
{

}

TraceBlockedByApertureError::~TraceBlockedByApertureError()
{

}

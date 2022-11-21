#include "sequential/ray_segment.h"

using namespace geopter;

RaySegment::RaySegment()
{
    intersect_pt_ = Eigen::Vector3d::Zero(3);
    normal_       = Eigen::Vector3d::Zero(3);
    after_dir_    = Eigen::Vector3d::Zero(3);
    path_length_  = 0.0;
    opl_  = 0.0;
    before_ = nullptr;
    status_ = TRACE_NOT_REACHED_ERROR;
}

RaySegment::RaySegment(int i, const Eigen::Vector3d& inc_pt, const Eigen::Vector3d& normal, const Eigen::Vector3d& after_dir, double dist, double opl, RaySegment* before)
{
    index_        = i;
    intersect_pt_ = inc_pt;
    normal_       = normal;
    after_dir_    = after_dir;
    path_length_  = dist;
    opl_          = opl;
    before_       = before;
    status_ = TRACE_NOT_REACHED_ERROR;
}

RaySegment::RaySegment(const RaySegment& other)
{
    intersect_pt_ = other.intersect_pt();
    normal_       = other.surface_normal();
    after_dir_    = other.after_dir();
    path_length_  = other.path_length();
    opl_          = other.optical_path_length();
    before_       = nullptr;
    status_       = other.status();
}

RaySegment::~RaySegment()
{
    before_ = nullptr;
}

void RaySegment::set_data(const Eigen::Vector3d &inc_pt, const Eigen::Vector3d &normal, const Eigen::Vector3d &after_dir, double dist, double opl)
{
    intersect_pt_ = inc_pt;
    normal_       = normal;
    after_dir_    = after_dir;
    path_length_  = dist;
    opl_          = opl;
}

double RaySegment::aoi() const
{
    Eigen::Vector3d inc_dir;

    if(before_) {
        inc_dir = before_->after_dir();
    }else{
        inc_dir = after_dir_;
    }

    // We need signed value
    double tanU1 = inc_dir(1)/inc_dir(2);
    double tanU2 = normal_(1)/normal_(2);
    double tanI = (tanU1 - tanU2)/(1.0 + tanU1*tanU2); // I = U1-U2
    return atan(tanI);
}

double RaySegment::aor() const
{
    double tanU1 = after_dir_(1)/after_dir_(2);
    double tanU2 = normal_(1)/normal_(2);
    double tanI_prime = (tanU1 - tanU2)/(1.0 + tanU1*tanU2);
    return atan(tanI_prime);
}

#include "sequential/ray_at_surface.h"

using namespace geopter;

RayAtSurface::RayAtSurface()
{
    intersect_pt_ = Eigen::Vector3d::Zero(3);
    normal_       = Eigen::Vector3d::Zero(3);
    after_dir_    = Eigen::Vector3d::Zero(3);
    distance_from_before_ = 0.0;
    opl_  = 0.0;
    before_ = nullptr;
}

RayAtSurface::RayAtSurface(const Eigen::Vector3d& inc_pt, const Eigen::Vector3d& normal, const Eigen::Vector3d& after_dir, double dist, double opl, RayAtSurface *before)
{
    intersect_pt_ = inc_pt;
    normal_       = normal;
    after_dir_    = after_dir;
    distance_from_before_ = dist;
    opl_    = opl;
    before_ = before;
}

RayAtSurface::~RayAtSurface()
{
    before_ = nullptr;
}

void RayAtSurface::set_data(const Eigen::Vector3d &inc_pt, const Eigen::Vector3d &normal, const Eigen::Vector3d &after_dir, double dist, double opl, RayAtSurface *before)
{
    intersect_pt_         = inc_pt;
    normal_               = normal;
    after_dir_            = after_dir;
    distance_from_before_ = dist;
    opl_                  = opl;
    before_               = before;
}

double RayAtSurface::aoi() const
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

double RayAtSurface::aor() const
{
    double tanU1 = after_dir_(1)/after_dir_(2);
    double tanU2 = normal_(1)/normal_(2);
    double tanI_prime = (tanU1 - tanU2)/(1.0 + tanU1*tanU2);
    return atan(tanI_prime);
}

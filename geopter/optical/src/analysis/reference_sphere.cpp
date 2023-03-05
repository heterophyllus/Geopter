#include "analysis/reference_sphere.h"

using namespace geopter;

ReferenceSphere::ReferenceSphere()
{
    ref_pt_  = Eigen::Vector3d::Zero(3);
    ref_dir_ = Eigen::Vector3d::Zero(3);
    radius_  = 0.0;
    exp_dist_parax_ = 0.0;
}

ReferenceSphere::ReferenceSphere(const Eigen::Vector3d& ref_pt, const Eigen::Vector3d& ref_dir, double radius, double exp_dist_parax)
{
    ref_pt_  = ref_pt;
    ref_dir_ = ref_dir;
    radius_  = radius;
    exp_dist_parax_ = exp_dist_parax;
}

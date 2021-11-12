#include "analysis/reference_sphere.h"

using namespace geopter;

ReferenceSphere::ReferenceSphere()
{
    image_pt_ = Eigen::Vector3d::Zero(3);
    ref_dir_  = Eigen::Vector3d::Zero(3);
    ref_sphere_radius_ = 0.0;
}

ReferenceSphere::ReferenceSphere(const Eigen::Vector3d& image_pt, const Eigen::Vector3d& ref_dir, double ref_sphere_radius)
{
    image_pt_ = image_pt;
    ref_dir_ = ref_dir;
    ref_sphere_radius_ = ref_sphere_radius;
}

void ReferenceSphere::set_image_pt(const Eigen::Vector3d &image_pt)
{
    image_pt_ = image_pt;
}

void ReferenceSphere::set_ref_dir(const Eigen::Vector3d &ref_dir)
{
    ref_dir_ = ref_dir;
}

void ReferenceSphere::set_ref_sphere_radius(double ref_sphere_radius)
{
    ref_sphere_radius_ = ref_sphere_radius;
}


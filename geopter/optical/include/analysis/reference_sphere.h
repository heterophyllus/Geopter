#ifndef REFERENCE_SPHERE_H
#define REFERENCE_SPHERE_H

#include "Eigen/Core"
#include "sequential/ray.h"

namespace geopter{

class ReferenceSphere
{
public:
    ReferenceSphere();
    ReferenceSphere(const Eigen::Vector3d& ref_pt, const Eigen::Vector3d& ref_dir, double radius, double exp_dist_parax);

    Eigen::Vector3d& ref_pt() { return ref_pt_; }
    Eigen::Vector3d& ref_dir() { return ref_dir_; }
    double radius() const { return radius_; }

    void set_ref_pt(const Eigen::Vector3d& ref_pt) { ref_pt_ = ref_pt; }
    void set_ref_dir(const Eigen::Vector3d& ref_dir) { ref_dir_ = ref_dir; }
    void set_radius(double radius) { radius_ = radius; }

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW


private:
    Eigen::Vector3d ref_pt_;
    Eigen::Vector3d ref_dir_;
    double radius_;
    double exp_dist_parax_;

    RayPtr chief_ray_;
    Eigen::Vector3d cr_exp_pt_;
};


} // namespace geopter


#endif //REFERENCE_SPHERE_H

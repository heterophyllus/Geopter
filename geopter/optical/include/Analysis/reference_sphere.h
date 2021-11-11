#ifndef REFERENCE_SPHERE_H
#define REFERENCE_SPHERE_H

#include "Eigen/Core"
#include "sequential/ray.h"
#include "assembly/surface.h"

namespace geopter{


class ReferenceSphere
{
public:
    ReferenceSphere();
    ReferenceSphere(const Eigen::Vector3d& image_pt, const Eigen::Vector3d& ref_dir, double ref_sphere_radius);

    inline Eigen::Vector3d image_pt() const;
    inline Eigen::Vector3d ref_dir() const;
    inline double ref_sphere_radius() const;

    void update(const Surface* srf, const Ray* chief_ray, double img_dist, double exp_parax_dist, double wvl);

    void set_image_pt(const Eigen::Vector3d& image_pt);
    void set_ref_dir(const Eigen::Vector3d& ref_dir);
    void set_ref_sphere_radius(double ref_sphere_radius);

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

private:

    Eigen::Vector3d image_pt_;
    Eigen::Vector3d ref_dir_;
    double ref_sphere_radius_;
};



Eigen::Vector3d ReferenceSphere::image_pt() const
{
    return image_pt_;
}

Eigen::Vector3d ReferenceSphere::ref_dir() const
{
    return ref_dir_;
}

double ReferenceSphere::ref_sphere_radius() const
{
    return ref_sphere_radius_;
}


} // namespace geopter


#endif //REFERENCE_SPHERE_H

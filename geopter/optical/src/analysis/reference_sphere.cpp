#include "analysis/reference_sphere.h"

using namespace geopter;

namespace{

struct ChiefRayExpSeg
{
    Eigen::Vector3d pt; //chief ray intersection with exit pupil plane
    Eigen::Vector3d dir; //direction cosine of the chief ray in exit pupil space
    double dist; //distance from interface to the exit pupil point
    //Surface* surf;
    Eigen::Vector3d before_pt;
    Eigen::Vector3d before_dir;
};

ChiefRayExpSeg transfer_to_exit_pupil(const Surface* srf, const RayAtSurface* ray_seg, double exp_dist_parax){

    Eigen::Vector3d inc_pt = ray_seg->intersect_pt();
    Eigen::Vector3d after_dir = ray_seg->after_dir();

    Eigen::Vector3d before_pt;
    Eigen::Vector3d before_dir;
    Transformation::transform_after_surface(before_pt, before_dir, srf, inc_pt, after_dir);

    double h = before_pt(1);
    double u = before_dir(1);
    constexpr double eps = 1.0e-14;

    ChiefRayExpSeg cr_exp_seg;

    double exp_dist;
    if(fabs(u) < eps){
        exp_dist = exp_dist_parax;
    }else{
        exp_dist  = -h/u;
    }

    cr_exp_seg.dist = exp_dist;
    cr_exp_seg.pt = before_pt + exp_dist*before_dir;
    cr_exp_seg.dir = before_dir;

    cr_exp_seg.before_pt = before_pt;
    cr_exp_seg.before_dir = before_dir;
    //cr_exp_seg.surf = srf;

    return cr_exp_seg;

}

} //namespace

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

void ReferenceSphere::update(const Surface *srf, const Ray *chief_ray, double img_dist, double exp_parax_dist, double wvl)
{
    const double foc = 0.0;
    ChiefRayExpSeg cr_exp_seg = transfer_to_exit_pupil(srf, chief_ray->at_lens_back(), exp_parax_dist);

    double dist = foc / chief_ray->back()->after_dir()(2);
    image_pt_ = chief_ray->back()->intersect_pt() - dist*chief_ray->back()->after_dir();

    Eigen::Vector3d img_pt = image_pt_;
    img_pt(2) += img_dist;

    Eigen::Vector3d ref_sphere_vec = img_pt - cr_exp_seg.pt;

    ref_sphere_radius_ = ref_sphere_vec.norm();
    ref_dir_ = ref_sphere_vec.normalized();
}

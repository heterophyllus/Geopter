#ifndef WAVE_ABERRATION_H
#define WAVE_ABERRATION_H

#include "system/optical_system.h"
#include "analysis/reference_sphere.h"
#include "data/grid_array.h"
#include "data/plot_data.h"

namespace geopter {


class WaveAberration
{
public:
    WaveAberration(OpticalSystem* opt_sys );
    ~WaveAberration();

    std::shared_ptr<PlotData> plot_opd_fan(Field* fld, int nrd);

private:

    double wave_abr_full_calc(const std::shared_ptr<Ray>& ray, const std::shared_ptr<Ray>& chief_ray);

    /** calculate equally inclined chord distance between 2 rays */
    double eic_distance(const Eigen::Vector3d& p, const Eigen::Vector3d& d, const Eigen::Vector3d& p0, const Eigen::Vector3d& d0);


    void transform_after_surface(Eigen::Vector3d& before_pt, Eigen::Vector3d& before_dir, const Surface* srf, const RayAtSurface* ray_seg);

    ReferenceSphere setup_reference_sphere(const std::shared_ptr<Ray>& chief_ray, const Eigen::Vector3d& cr_exp_pt, const Eigen::Vector2d& image_pt_2d);
    ReferenceSphere setup_reference_sphere(const std::shared_ptr<Ray>& chief_ray, const Eigen::Vector3d& cr_exp_pt);

    void get_chief_ray_exp_segment(Eigen::Vector3d& cr_exp_pt, double& cr_exp_dist, const std::shared_ptr<Ray> chief_ray);

    OpticalSystem* opt_sys_;
};

}


#endif //WAVE_ABERRATION_H

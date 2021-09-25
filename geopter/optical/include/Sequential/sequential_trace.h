#ifndef SEQUENTIALTRACE_H
#define SEQUENTIALTRACE_H

#include <memory>

#include "Eigen/Core"

#include "Sequential/ray.h"
#include "Sequential/sequential_path.h"
#include "System/optical_system.h"
#include "Spec/field.h"

namespace geopter {

class SequentialTrace
{
public:
    SequentialTrace(OpticalSystem* sys);
    ~SequentialTrace();

    //std::shared_ptr<Ray> trace_pupil_ray(Eigen::Vector2d pupil_crd, int fi, int wi);

    std::shared_ptr<Ray> trace_pupil_ray(const Eigen::Vector2d& pupil_crd, const Field* fld, double wvl);

    /** Trace a ray throughout the given sequantial path */
    std::shared_ptr<Ray> trace_ray_throughout_path(const SequentialPath& seq_path, const Eigen::Vector3d& pt0, const Eigen::Vector3d& dir0);

    Eigen::Vector2d trace_coddington(const Field* fld, double wvl);

    //Eigen::Vector2d aim_chief_ray(int fi, int wi);

    Eigen::Vector2d aim_chief_ray(const Field* fld, double wvl);

    /**
     * @brief Search aim point of the ray to xy_target on the given surface
     * 
     * @param srf_idx target surface index
     * @param xy_target target coordinate
     * @param fi field index
     * @param wi wavelength index
     * @return Eigen::Vector2d aim point on paraxial entrance pupil plane
     */
    Eigen::Vector2d search_aim_point(int srf_idx, const Eigen::Vector2d& xy_target, const Field* fld, double wvl);
    //Eigen::Vector2d search_aim_point(int srf_idx, Eigen::Vector2d xy_target, int fi, int wi);

    /** @brief Refract incoming direction, d_in, about normal
     *  @param d_in incident direction
     *  @param normal normal of the surface
     *  @return direction of refracted ray
     */
    Eigen::Vector3d bend(Eigen::Vector3d d_in, Eigen::Vector3d normal, double n_in, double n_out);

    /** Get object coordinate for the given field */
    //Eigen::Vector3d object_coord(int fi);

    Eigen::Vector3d object_coord(const Field* fld);

    /** Get sequential path between start and end */
    SequentialPath sequential_path(int start, int end, double wvl);

    /** Get sequential path object from object to image */
    SequentialPath overall_sequential_path(double wvl);

    std::vector<double> compute_vignetting_factors(const Field& fld);


    void set_aperture_check(bool state);
    bool aperture_check_state() const;

    void set_apply_vig(bool state);
    bool apply_vig_status() const;

private:
    double y_stop_coordinate(double y1, int ifcx, Eigen::Vector3d pt0, double dist, double wvl, double y_target);

    double compute_vignetting_factor_for_pupil(Eigen::Vector2d full_pupil, const Field& fld);
    
    OpticalSystem *opt_sys_;

    int num_wvls_;
    int num_flds_;
    double ref_wvl_val_;
    int ref_wvl_idx_;

    int num_srfs_;
    int num_gaps_;
    int image_index_;
    double object_distance_;

    bool do_aperture_check_;

    bool do_apply_vig_;
};

}

#endif // SEQUENTIALTRACE_H

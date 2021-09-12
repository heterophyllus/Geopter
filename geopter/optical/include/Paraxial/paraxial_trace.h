#ifndef PARAXIALTRACE_H
#define PARAXIALTRACE_H

#include "paraxial_path.h"
#include "paraxial_ray.h"


namespace geopter {

class OpticalSystem;

class ParaxialTrace
{
public:
    ParaxialTrace(OpticalSystem* sys);
    ~ParaxialTrace();

    /** 
     * @brief Do paraxial trace throughout the system
     * @param par_path paraxial path
     * @param y0 paraxial ray height at starting point
     * @param u0 slope of outgoing ray at starting point
     */
    ParaxialRay trace_paraxial_ray_from_object(double y0, double u0, double wvl);

    ParaxialRay trace_paraxial_ray_aiming_at_surface(int target_srf, double y_target, double u_prime_target ,double wvl);

    /** trace [y1,u1] = [1.0, 0.0] */
    ParaxialRay trace_paraxial_ray_parallel_to_axis_at_s1();

    /** trace [y1,u1] = [0.0, uq0] */
    ParaxialRay trace_paraxial_ray_with_slope_at_s1();
    /**
     * @brief Generate the paraxial path 
     * 
     * @param start start surface index
     * @param end end surface index
     * @param wi wavelength index
     * @return ParaxialPath 
     */
    ParaxialPath paraxial_path(int start, int end, double wvl);

    //void get_starting_coords(double *y, double *u, double *ybar, double *ubar);

private:
    ParaxialPath forward_paraxial_path(int start, int end, double wvl);
    ParaxialPath reverse_paraxial_path(int start, int end, double wvl);

    void compute_starting_data();

    OpticalSystem* opt_sys_;

    double ref_u0_;
    double ref_y0_;
    double ref_ubar0_;
    double ref_ybar0_;
};

} // namespace geopter

#endif // PARAXIALTRACE_H

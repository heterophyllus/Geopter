#ifndef TRACE_H
#define TRACE_H

#include "ray.h"
#include "sequential_model.h"
#include "Eigen/Core"


namespace geopter {

class Field;

class Trace
{
public:
    Trace();

    /** Trace ray specified by relative aperture and field point */
    static Ray trace_pupil_ray(const OpticalModel& opt_model, Eigen::Vector2d pupil, const Field& fld, double wvl);

    static Ray trace_pupil_ray(const OpticalModel& opt_model, Eigen::Vector2d pupil, int fi, int wi);

    static std::vector<Ray> trace_fan_rays(const OpticalModel& opt_model, const Field& fld, double wvl, double azimuth, int nrd);


    static Eigen::Vector2d aim_chief_ray(const OpticalModel& opt_model, const Field& fld, double wvl=0.0);

    /** @brief iterates a ray to xy_target on interface ifcx, returns aim points on the paraxial entrance pupil plane
      * @note If idcx is None, i.e. a floating stop surface, returns xy_target.
      *       If the iteration fails, a TraceError will be raised
      */
    static Eigen::Vector2d iterate_ray(const OpticalModel& opt_model, int ifcx, Eigen::Vector2d xy_target, const Field& fld, double wvl);

    static void apply_paraxial_vignetting(const OpticalModel& opt_model);


    /** Trace astigmatism using Coddington equation */
    static Eigen::Vector2d trace_coddington(const OpticalModel& opt_model, const Field& fld, double wvl);


    /** refract incoming direction, d_in, about normal */
    static Eigen::Vector3d bend(Eigen::Vector3d d_in, Eigen::Vector3d normal, double n_in, double n_out);

    /** reflect incoming direction, d_in, about normal */
    static Eigen::Vector3d reflect(Eigen::Vector3d d_in, Eigen::Vector3d normal);

    /** Trace ray originated from the given point in object space */
    static Ray trace_ray_from_object(const SequentialModel& seq_model, Eigen::Vector3d pt0, Eigen::Vector3d dir0, double wvl);

    /** Trace ray originated from the given point in object space */
    static Ray trace_ray_from_object(Path path, Eigen::Vector3d pt0, Eigen::Vector3d dir0, double wvl, double eps=1.0e-12);

private:

    /** internal function called in iterate_ray() */
    static double y_stop_coordinate(double y1, const SequentialModel& seq_model, int ifcx, Eigen::Vector3d pt0, double dist, double wvl, double y_target);

    static bool in_surface_range(int s, int start_surf=0, int last_surf=0, bool include_last_surf=false);

};

} //namespace geopter

#endif // TRACE_H

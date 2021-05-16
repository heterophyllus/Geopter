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

    /**
     * @brief Trace ray specified by relative aperture and field point
     * @param opt_model instance of :class:`~.OpticalModel` to trace
     * @param pupil relative pupil coordinates of ray
     * @param fld instance of :class:`~.Field`
     * @param wvl ray trace wavelength in nm
     * @return
     */
    static Ray trace_base(const OpticalModel& opt_model, Eigen::Vector2d pupil, const Field& fld, double wvl);

    static Ray trace_base(const OpticalModel& opt_model, Eigen::Vector2d pupil, int fld_idx, double wvl);

    // /** returns a list of RayPkgs for the boundary rays for field fld */
    //static RayPkgs trace_boundary_rays_at_field(OpticalModel *opt_model, Field *fld, double wvl, bool use_named_tuples=false);

    //static RaySet trace_boundary_rays(OpticalModel* opt_model, bool use_named_tuples=false);


    static Eigen::Vector2d aim_chief_ray(const OpticalModel& opt_model, const Field& fld, double wvl=0.0);

    /** @brief iterates a ray to xy_target on interface ifcx, returns aim points on the paraxial entrance pupil plane
      * @note If idcx is None, i.e. a floating stop surface, returns xy_target.
      *       If the iteration fails, a TraceError will be raised
      */
    static Eigen::Vector2d iterate_ray(const OpticalModel& opt_model, int ifcx, Eigen::Vector2d xy_target, const Field& fld, double wvl);

    static void apply_paraxial_vignetting(const OpticalModel& opt_model);


    // belows are from ray_trace.h


    /** refract incoming direction, d_in, about normal */
    static Eigen::Vector3d bend(Eigen::Vector3d d_in, Eigen::Vector3d normal, double n_in, double n_out);

    /** reflect incoming direction, d_in, about normal */
    static Eigen::Vector3d reflect(Eigen::Vector3d d_in, Eigen::Vector3d normal);

    /**
     * @brief fundamental raytrace function
     * @param seq_model the sequential model to be traced
     * @param pt0 starting point in coords of first interface
     * @param dir0 starting direction cosines in coords of first interface
     * @param wvl wavelength in nm
     * @return
     */
    static Ray trace(const SequentialModel& seq_model, Eigen::Vector3d pt0, Eigen::Vector3d dir0, double wvl);

    /**
     * @brief fundamental raytrace function
     * @param path an iterator containing interfaces and gaps to be traced.
              for each iteration, the sequence or generator should return a
              list containing: **Intfc, Gap, Trfm, Index, Z_Dir
     * @param pt0 starting point in coords of first interface
     * @param dir0 starting direction cosines in coords of first interface
     * @param wvl wavelength in nm
     * @param eps accuracy tolerance for surface intersection calculation
     * @return (**ray**, **op_delta**, **wvl**)
     */
    static Ray trace_raw(Path path, Eigen::Vector3d pt0, Eigen::Vector3d dir0, double wvl, double eps=1.0e-12);

private:

    /** internal function called in iterate_ray() */
    static double y_stop_coordinate(double y1, const SequentialModel& seq_model, int ifcx, Eigen::Vector3d pt0, double dist, double wvl, double y_target);

    static bool in_surface_range(int s, int start_surf=0, int last_surf=0, bool include_last_surf=false);

};

} //namespace geopter

#endif // TRACE_H

#ifndef PARAXIALMODEL_H
#define PARAXIALMODEL_H

#include "Eigen/Core"
#include "paraxial_path.h"
#include "paraxial_ray.h"
#include "first_order.h"

namespace geopter {

class OpticalModel;

class ParaxialModel
{
public:
    ParaxialModel(OpticalModel* opt_model);
    ~ParaxialModel();

    void update_model();

    /** @brief Returns first order data
     *  @note calculated in the reference wavelength
     */
    FirstOrderData first_order_data() const;

    /** Returns axial ray */
    ParaxialRay ax() const;

    /** Returns principle ray */
    ParaxialRay pr() const;

    FirstOrderData compute_first_order();

    /** Returns Kingslake system matrix of the partial system between start and end */
    Eigen::Matrix2d compute_system_matrix(int start, int end, double wvl) const;

    /** Get first and second nodal points of the partial system  */
    void nodal_points(int start, int end, double* pp1, double* pp2) const;

    /** trace both paraxial rays from object to surface in the given wavelength */
    void compute_paraxial_ray(ParaxialRay& ax_ray, ParaxialRay& pr_ray, double wvl);

    /** Get paraxial path */
    ParaxialPath paraxial_path(int start, int end, double wvl) const;


private:
    /** Returns distance from 1st surface to entrance pupil */
    double compute_entrance_pupil_distance() const;

    /** Returns distance from last(SI-1) surface to exit pupil */
    double compute_exit_pupil_distance() const;

    ParaxialPath forward_paraxial_path(int start, int end, double wvl) const;
    ParaxialPath reverse_paraxial_path(int start, int end, double wvl) const;

    OpticalModel* opt_model_;
    double opt_inv_;

    ParaxialRay ax_;
    ParaxialRay pr_;

    FirstOrderData fod_;
};

} //namespace geopter

#endif // PARAXIALMODEL_H

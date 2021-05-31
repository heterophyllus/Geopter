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

    FirstOrderData first_order_data() const;
    ParaxialRay ax() const;
    ParaxialRay pr() const;


    //void paraxial_trace(ParaxialRay& p_ray, ParaxialRay& p_ray_bar, int start, ParaxialRayHtAndSlp start_yu, ParaxialRayHtAndSlp start_yu_bar, double wvl);


    FirstOrderData compute_first_order();

    /** Returns Kingslake system matrix of the partial system between start and end */
    Eigen::Matrix2d compute_system_matrix(int start, int end, double wvl);

    /** Get first and second nodal points of the partial system  */
    void nodal_points(int start, int end, double* pp1, double* pp2);

    void compute_paraxial_ray(ParaxialRay& ax_ray, ParaxialRay& pr_ray, double wvl);

    ParaxialPath forward_paraxial_path(int start, int end, double wvl);
    ParaxialPath reverse_paraxial_path(int start, int end, double wvl);

private:
    /** Returns distance from 1st surface to entrance pupil */
    double compute_entrance_pupil_distance();

    /** Returns distance from last(SI-1) surface to exit pupil */
    double compute_exit_pupil_distance();

    OpticalModel* opt_model_;
    double opt_inv_;

    ParaxialRay ax_;
    ParaxialRay pr_;

    FirstOrderData fod_;
};

} //namespace geopter

#endif // PARAXIALMODEL_H

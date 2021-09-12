#ifndef PARAXIALMODEL_H
#define PARAXIALMODEL_H

#include <vector>

#include "Eigen/Core"

#include "paraxial_ray.h"
#include "first_order.h"

namespace geopter {

class OpticalSystem;

class ParaxialModel
{
public:
    ParaxialModel(OpticalSystem* sys);
    ~ParaxialModel();

    void update_model();

    FirstOrderData first_order_data() const;

    /** Returns axial ray */
    ParaxialRay axial_ray(int wi) const;

    /** Returns principle ray */
    ParaxialRay principle_ray(int wi) const;

    /** Returns Kingslake system matrix of the partial system between start and end */
    Eigen::Matrix2d system_matrix(int start, int end, int wi) const;


private:
    void compute_first_order(bool update_parax_rays= false);

    /** trace ax/pr rays */
    void compute_paraxial_rays();

    void get_starting_coords(double *y, double *u, double *ybar, double *ubar);

    OpticalSystem* opt_sys_;

    /** parallel to axis at s1 */
    ParaxialRay p_ray_;

    /** with slope at s1 */
    ParaxialRay q_ray_;

    /** paraxial axial rays computed with all wavelengths */
    std::vector<ParaxialRay> ax_rays_;

    /** paraxial principle rays computed with all wavelengths */
    std::vector<ParaxialRay> pr_rays_;

    /** first order data computed with all wavelengths */
    FirstOrderData fod_;
};

} //namespace geopter

#endif // PARAXIALMODEL_H

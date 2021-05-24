#ifndef PARAXIALMODEL_H
#define PARAXIALMODEL_H


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

    void compute_paraxial_ray(ParaxialRay& ax_ray, ParaxialRay& pr_ray, double wvl);
    void paraxial_trace(ParaxialRay& p_ray, ParaxialRay& p_ray_bar, int start, ParaxialRayHtAndSlp start_yu, ParaxialRayHtAndSlp start_yu_bar, double wvl);

    FirstOrderData compute_first_order();


private:
    OpticalModel* opt_model_;
    double opt_inv_;

    ParaxialRay ax_;
    ParaxialRay pr_;

    FirstOrderData fod_;
};

} //namespace geopter

#endif // PARAXIALMODEL_H

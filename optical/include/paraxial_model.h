#ifndef PARAXIALMODEL_H
#define PARAXIALMODEL_H


#include "paraxial_ray.h"

namespace geopter {


class OpticalModel;

struct FirstOrderData{
    double opt_inv;
    double power;
    double efl;
    double pp1;
    double ppk;
    double ffl;
    double bfl;
    double fno;
    double m;
    double red;
    double n_obj;
    double n_img;
    double obj_dist;
    double img_dist;
    double obj_ang;
    double img_ht;
    double enp_dist;
    double enp_radius;
    double exp_dist;
    double exp_radius;
    double obj_na;
    double img_na;

    void print();
    void print(std::ostringstream& oss);
};


class ParaxialModel
{
public:
    ParaxialModel(OpticalModel* opt_model);
    ~ParaxialModel();

    void update_model();

    FirstOrderData first_order_data() const;
    ParaxialRay ax() const;
    ParaxialRay pr() const;

    void paraxial_trace(ParaxialRay& p_ray, ParaxialRay& p_ray_bar, int start, ParaxialRayHtAndSlp start_yu, ParaxialRayHtAndSlp start_yu_bar);

    void compute_first_order(int stop=0, double wvl=587);

private:
    void build_lens();

private:
    OpticalModel* opt_model_;
    double opt_inv_;

    ParaxialRay ax_;
    ParaxialRay pr_;

    FirstOrderData fod_;
};

} //namespace geopter

#endif // PARAXIALMODEL_H

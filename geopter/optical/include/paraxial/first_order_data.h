#ifndef FIRST_ORDER_DATA_H
#define FIRST_ORDER_DATA_H

#include <sstream>
#include <iomanip>

namespace geopter
{

class OpticalSystem;

struct FirstOrderData
{
    FirstOrderData(OpticalSystem* parent);
    ~FirstOrderData();

    double ref_y0;
    double ref_ybar0;
    double ref_u0;
    double ref_ubar0;
    double opt_inv;
    double efl;
    double fno;
    double red;
    double enp_dist;
    double enp_radius;
    double exp_dist;
    double exp_radius;
    double img_dist;
    double obj_dist;
    double pp1;
    double ppk;
    double ffl;
    double bfl;
    double n_obj;
    double n_img;
    double img_ht;
    double obj_ang;
    double obj_na;
    double img_na;

    void update();

    void print(std::ostringstream& oss);

private:
    OpticalSystem* parent_;
};





}


#endif

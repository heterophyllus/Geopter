#ifndef FIRSTORDERDATA_H
#define FIRSTORDERDATA_H

#include <sstream>

namespace geopter {

/** First order data container */
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

}

#endif // FIRSTORDERDATA_H

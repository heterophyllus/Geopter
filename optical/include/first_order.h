#ifndef FIRSTORDERDATA_H
#define FIRSTORDERDATA_H

#include <sstream>

namespace geopter {

/** First order data container */
struct FirstOrderData{
    /** optical invariant */
    double opt_inv;

    /** effective focla length */
    double efl;

    /** front focas */
    double ffl;

    /** back focus */
    double bfl;

    /** F number */
    double fno;

    /** actual image distance */
    double img_dist;

    /** maximum incident angle in objective space */
    double obj_ang;

    /** paraxial image height */
    double img_ht;

    /** entrance pupil distance (from s1) */
    double enp_dist;

    /** entrance pupil radius */
    double enp_radius;

    /** exit pupil distance (from sI-1) */
    double exp_dist;

    /** exit pupil radius */
    double exp_radius;


    // belows are computed when the system is finite conjugate
    /** magnification */
    double m;

    /** reduction rate */
    double red;

    /** object distance */
    double obj_dist;

    void print();
    void print(std::ostringstream& oss);
};

}

#endif // FIRSTORDERDATA_H

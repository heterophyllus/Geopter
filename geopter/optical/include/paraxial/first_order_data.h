#ifndef FIRST_ORDER_DATA_H
#define FIRST_ORDER_DATA_H

#include <sstream>
#include <iomanip>

namespace geopter
{

struct FirstOrderData
{
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

    void print(std::ostringstream& oss){
        constexpr int fixed_w = 30;
        constexpr int pre = 4;

        oss << std::setw(fixed_w) << std::left << "Effective Focal Length";
        oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << efl << std::endl;

        oss << std::setw(fixed_w) << std::left << "Front Focal Length";
        oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << ffl << std::endl;

        oss << std::setw(fixed_w) << std::left << "Back Focal Length";
        oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << bfl << std::endl;

        oss << std::setw(fixed_w) << std::left << "F/#";
        oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << fno << std::endl;

        oss << std::setw(fixed_w) << std::left << "Reduction Rate";
        oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << red << std::endl;

        oss << std::setw(fixed_w) << std::left << "Object Distance";
        oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << obj_dist << std::endl;

        oss << std::setw(fixed_w) << std::left << "Object Angle";
        oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << obj_ang << std::endl;

        oss << std::setw(fixed_w) << std::left << "Image Distance";
        oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << img_dist << std::endl;

        oss << std::setw(fixed_w) << std::left << "Image Height";
        oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << img_ht << std::endl;

        oss << std::setw(fixed_w) << std::left << "Entrance Pupil Distance";
        oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << enp_dist << std::endl;

        oss << std::setw(fixed_w) << std::left << "Entrance Pupil Radius";
        oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << enp_radius << std::endl;

        oss << std::setw(fixed_w) << std::left << "Exit Pupil Distance";
        oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << exp_dist << std::endl;

        oss << std::setw(fixed_w) << std::left << "Exit Pupil Radius";
        oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << exp_radius << std::endl;

        oss << std::setw(fixed_w) << std::left << "Optical Invariant";
        oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << opt_inv << std::endl;

        oss << std::endl;
    }
};





}


#endif

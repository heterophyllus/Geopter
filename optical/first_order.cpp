#include <iostream>
#include <iomanip>

#include "first_order.h"

using namespace geopter;

void FirstOrderData::print()
{
    std::ostringstream oss;
    print(oss);
    std::cout << oss.str() << std::endl;
}

void FirstOrderData::print(std::ostringstream& oss)
{
    const int fixed_w = 20;
    const int pre = 4;

    oss << std::setw(fixed_w) << std::left << "efl";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << efl << std::endl;

    oss << std::setw(fixed_w) << std::left << "ffl";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << ffl << std::endl;

    oss << std::setw(fixed_w) << std::left << "bfl";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << bfl << std::endl;

    oss << std::setw(fixed_w) << std::left << "f/#";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << fno << std::endl;

    oss << std::setw(fixed_w) << std::left << "red";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << red << std::endl;

    oss << std::setw(fixed_w) << std::left << "obj_dist";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << obj_dist << std::endl;

    oss << std::setw(fixed_w) << std::left << "obj_ang";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << obj_ang << std::endl;

    oss << std::setw(fixed_w) << std::left << "enp_dist";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << enp_dist << std::endl;

    oss << std::setw(fixed_w) << std::left << "enp_radius";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << enp_radius << std::endl;

    oss << std::setw(fixed_w) << std::left << "img_dist";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << img_dist << std::endl;

    oss << std::setw(fixed_w) << std::left << "img_ht";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << img_ht << std::endl;

    oss << std::setw(fixed_w) << std::left << "exp_dist";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << exp_dist << std::endl;

    oss << std::setw(fixed_w) << std::left << "exp_radius";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << exp_radius << std::endl;

    oss << std::setw(fixed_w) << std::left << "opt inv";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << opt_inv << std::endl;

    oss << std::ends;
}

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

    double reference_y0;
    double reference_ybar0;
    double reference_u0;
    double reference_ubar0;
    double optical_invariant;
    double effective_focal_length;
    double fno;
    double reduction;
    double entrance_pupil_distance;
    double entrance_pupil_radius;
    double exit_pupil_distance;
    double exit_pupil_radius;
    double image_distance;
    double object_distance;
    double pp1;
    double ppk;
    double front_focal_length;
    double back_focal_length;
    double n_obj;
    double n_img;
    double image_height;
    double object_angle;
    double object_space_na;
    double image_space_na;

    void Update();

    void Print(std::ostringstream& oss);

private:
    OpticalSystem* parent_;
};


}


#endif

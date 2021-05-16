#ifndef DECENTERDATA_H
#define DECENTERDATA_H

#include "transform.h"

#include "Eigen/Core"
#include "Eigen/Geometry"


namespace geopter {


enum DecType{
    LOCAL,
    REV,
    DAR,
    BEND
};

class DecenterData
{
    /**
    * @brief Maintains data and actions for position and orientation changes.
    *
        - LOCAL: pos and orientation applied prior to surface
        - REV:   pos and orientation applied following surface in reverse
        - DAR:   pos and orientation applied prior to surface and then returned to initial frame
        - BEND:  used for fold mirrors, orientation applied before and after surface
    */
public:
    DecenterData(int dtype, double x=0.0, double y=0.0, double alpha=0.0, double beta=0.0, double gamma=0.0);
    ~DecenterData();

    Tfrm tfrom_before_surf();
    Tfrm tform_after_surf();

    void update();

    Eigen::Matrix3d euler2mat(double ai, double aj, double ak);

private:

    int dectype_;

    // vertex decenter
    double x_;
    double y_;
    Eigen::Vector3d dec_;

    // euler angle
    double alpha_;
    double beta_;
    double gamma_;
    Eigen::Vector3d euler_;

    // rotation point offset
    Eigen::Vector3d rot_pt_;
    Eigen::Matrix3d rot_mat_;
};

} //namespace geopter

#endif // DECENTERDATA_H

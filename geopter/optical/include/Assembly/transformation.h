#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Eigen/Core"

namespace geopter{

struct Transformation
{
    Transformation() {
        rotation = Eigen::Matrix3d::Identity(3,3);
        transfer = Eigen::Vector3d::Zero(3);
    }
    Transformation(Eigen::Matrix3d r, Eigen::Vector3d t) {
        rotation = r;
        transfer = t;
    }

    Eigen::Matrix3d rotation;
    Eigen::Vector3d transfer;
};


} //namespace geopter

#endif // TRANSFORM_H

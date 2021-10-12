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
    Transformation(const Eigen::Matrix3d& r, const Eigen::Vector3d& t) {
        rotation = r;
        transfer = t;
    }

    Eigen::Matrix3d rotation;
    Eigen::Vector3d transfer;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


} //namespace geopter

#endif // TRANSFORM_H

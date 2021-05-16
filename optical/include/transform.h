#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Eigen/Core"

namespace geopter{

struct Tfrm
{
    Eigen::Matrix3d rotation;
    Eigen::Vector3d transfer;
};

class Transform
{
public:
    Transform();
};

} //namespace geopter

#endif // TRANSFORM_H

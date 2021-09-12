#define _USE_MATH_DEFINES
#include <math.h>

#include "Assembly/decenter_data.h"

using namespace geopter;

DecenterData::DecenterData(int dtype, double x, double y, double alpha, double beta, double gamma) :
    dectype_(dtype),
    x_(x),
    y_(y),
    alpha_(alpha),
    beta_(beta),
    gamma_(gamma)
{
    dec_   = {x, y, 0.0};
    euler_ = {alpha, beta, gamma};

    rot_pt_  = Eigen::Vector3d::Zero(3);
    rot_mat_ = Eigen::Matrix3d::Zero(3,3); // should be None
}

DecenterData::~DecenterData()
{

}

void DecenterData::update()
{
    double ai = -euler_(0)* M_PI/180.0;
    double aj = -euler_(1)* M_PI/180.0;
    double ak =  euler_(2)* M_PI/180.0;

    rot_mat_ = euler2mat(ai, aj, ak);
}

Transformation DecenterData::tfrom_before_surf()
{
    Transformation ret;

    if(dectype_ != DecType::REV){
        ret.rotation = rot_mat_;
        ret.transfer = dec_;

        return ret;
    }
    else{
        ret.rotation = Eigen::Matrix3d::Zero(3,3); // None
        ret.transfer = Eigen::Vector3d::Zero(3);
    }

    return ret;
}

Transformation DecenterData::tform_after_surf()
{
    Transformation ret;

    if( (dectype_ == DecType::REV) || (dectype_ == DecType::DAR) )
    {
        auto rt = rot_mat_;
        if(rot_mat_ != Eigen::Matrix3d::Zero(3,3)){ // is not None
            rt = rot_mat_.transpose();
        }
        ret.rotation = rt;
        ret.transfer = - dec_;
        return ret;
    }
    else if(dectype_ == DecType::BEND){
        ret.rotation = rot_mat_;
        ret.transfer = {0.0, 0.0, 0.0};
        return ret;
    }
    else{
        ret.rotation = Eigen::Matrix3d::Zero(3,3);
        ret.transfer = {0.0, 0.0, 0.0};
        return ret;
    }

}

Eigen::Matrix3d DecenterData::euler2mat(double ai, double aj, double ak)
{
    double roll  = ai;
    double pitch = aj;
    double yaw   = ak;

    Eigen::Matrix3d rot;
    rot = Eigen::AngleAxisd(roll, Eigen::Vector3d::UnitX())
                        * Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY())
                        * Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ());
    return rot;
}

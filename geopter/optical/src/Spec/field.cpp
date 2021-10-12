#include "Spec/field.h"

using namespace geopter;

Field::Field(double x, double y, double wt, Rgb color, double vuy, double vly, double vux, double vlx) :
    x_(x),
    y_(y),
    wt_(wt),
    vux_(vux),
    vuy_(vuy),
    vlx_(vlx),
    vly_(vly),
    render_color_(color)
{
    aim_pt_ = Eigen::Vector2d::Zero(2);
    object_coord_ = Eigen::Vector3d::Zero(3);
}

Field::~Field()
{

}


Eigen::Vector2d Field::apply_vignetting(const Eigen::Vector2d& pupil) const
{
    Eigen::Vector2d vig_pupil = pupil;

    if(pupil(0) < 0.0){ 
        vig_pupil(0) *= (1.0 - vlx_);
    }else{ 
        vig_pupil(0) *= (1.0 - vux_);
    }

    if(pupil(1) < 0.0){ 
        vig_pupil(1) *= (1.0 - vly_);
    }else{  
        vig_pupil(1) *= (1.0 - vuy_);
    }

    return vig_pupil;
}



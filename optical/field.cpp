#include "field.h"

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
}

Field::~Field()
{

}

double Field::x() const
{
    return x_;
}

double Field::y() const
{
    return y_;
}

double Field::vux() const
{
    return vux_;
}

double Field::vlx() const
{
    return vlx_;
}

double Field::vly() const
{
    return vly_;
}

double Field::vuy() const
{
    return vuy_;
}

double Field::weight() const
{
    return wt_;
}

Rgb Field::render_color() const
{
    return render_color_;
}

void Field::set_x(double x)
{
    x_ = x;
}

void Field::set_y(double y)
{
    y_ = y;
}

void Field::set_vlx(double vlx)
{
    vlx_ = vlx;
}

void Field::set_vux(double vux)
{
    vux_ = vux;
}

void Field::set_vly(double vly)
{
    vly_ = vly;
}

void Field::set_vuy(double vuy)
{
    vuy_ = vuy;
}

void Field::set_weight(double wt)
{
    wt_ = wt;
}

void Field::set_render_color(const Rgb& color)
{
    render_color_ = color;
}

PupilCrd Field::aim_pt() const
{
    return aim_pt_;
}

void Field::set_aim_pt(PupilCrd aim_pt)
{
    aim_pt_ = aim_pt;
}

PupilCrd Field::apply_vignetting(PupilCrd pupil) const
{
    PupilCrd vig_pupil = pupil;

    if(pupil(0) < 0.0){
        if(vlx_ != 0.0){
            vig_pupil(0) *= (1.0 - vlx_);
        }
    }else{
        if(vux_ != 0.0){
            vig_pupil(0) *= (1.0 - vux_);
        }
    }

    if(pupil(1) < 0.0){
        if(vly_ != 0.0){
            vig_pupil(1) *= (1.0 - vly_);
        }
    }
    else{
        if(vuy_ != 0.0){
            vig_pupil(1) *= (1.0 - vuy_);
        }
    }

    return vig_pupil;
}

void Field::update()
{
    //_chief_ray = nullptr;
}

#ifndef FIELD_H
#define FIELD_H


#include "Renderer/rgb.h"
#include "Eigen/Core"

namespace geopter {


/** Container class as a component of field of view */
class Field
{
public:
    Field(double x=0.0, double y=0.0, double wt=1.0, Rgb color= rgb_black, double vuy=0.0, double vly=0.0, double vux=0.0, double vlx=0.0);
    ~Field();

    inline double x() const;
    inline double y() const;
    inline double vlx() const;
    inline double vux() const;
    inline double vly() const;
    inline double vuy() const;
    inline double weight() const;
    inline Rgb render_color() const;
    inline Eigen::Vector2d aim_pt() const;
    inline Eigen::Vector3d object_coord() const;

    inline void set_x(double x);
    inline void set_y(double y);
    inline void set_vlx(double vlx);
    inline void set_vux(double vux);
    inline void set_vly(double vly);
    inline void set_vuy(double vuy);
    inline void set_weight(double wt);
    inline void set_render_color(const Rgb& color);
    inline void set_aim_pt(const Eigen::Vector2d& aim_pt);
    inline void set_object_coord(const Eigen::Vector3d& obj_crd);

    Eigen::Vector2d apply_vignetting(const Eigen::Vector2d& pupil) const;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

private:
    double x_;
    double y_;
    double wt_;
    double vux_;
    double vuy_;
    double vlx_;
    double vly_;
    Eigen::Vector2d aim_pt_;
    Eigen::Vector3d object_coord_;
    Rgb render_color_;
};



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


Eigen::Vector3d Field::object_coord() const
{
    return object_coord_;
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

Eigen::Vector2d Field::aim_pt() const
{
    return aim_pt_;
}

void Field::set_aim_pt(const Eigen::Vector2d& aim_pt)
{
    aim_pt_ = aim_pt;
}

void Field::set_object_coord(const Eigen::Vector3d& obj_crd)
{
    object_coord_ = obj_crd;
}


} //namespace geopter

#endif // FIELD_H

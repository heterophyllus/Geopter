#ifndef FIELD_H
#define FIELD_H


#include "Renderer/rgb.h"
#include "Eigen/Core"

namespace geopter {

using PupilCrd = Eigen::Vector2d;


/** Container class as a component of field of view */
class Field
{
public:
    Field(double x=0.0, double y=0.0, double wt=1.0, Rgb color= rgb_black, double vuy=0.0, double vly=0.0, double vux=0.0, double vlx=0.0);
    ~Field();

    double x() const;
    double y() const;
    double vlx() const;
    double vux() const;
    double vly() const;
    double vuy() const;
    double weight() const;
    Rgb render_color() const;

    void set_x(double x);
    void set_y(double y);
    void set_vlx(double vlx);
    void set_vux(double vux);
    void set_vly(double vly);
    void set_vuy(double vuy);
    void set_weight(double wt);
    void set_render_color(const Rgb& color);

    Eigen::Vector2d aim_pt() const;
    void set_aim_pt(PupilCrd aim_pt);

    PupilCrd apply_vignetting(PupilCrd pupil) const;

    void update();

protected:
    double x_;
    double y_;
    double vux_;
    double vuy_;
    double vlx_;
    double vly_;
    double wt_;

    Eigen::Vector2d aim_pt_;

    Rgb render_color_;
};

} //namespace geopter

#endif // FIELD_H

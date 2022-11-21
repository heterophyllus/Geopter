/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
** 
** This file is part of Geopter.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
** 
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
** 
** You should have received a copy of the GNU General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi                                   
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com                          
**             Date: May 16th, 2021                                                                                          
********************************************************************************/


#ifndef FIELD_H
#define FIELD_H

#include "Eigen/Core"

#include "renderer/rgb.h"

namespace geopter {

class Field
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    Field();
    Field(double x, double y, const Rgb& color);
    Field(double x, double y, double wt, const Rgb& color, double vuy, double vly, double vux, double vlx);
    ~Field();

    double x() const                  { return x_; }
    double y() const                  { return y_; }
    double vlx() const                { return vlx_; }
    double vux() const                { return vux_; }
    double vly() const                { return vly_; }
    double vuy() const                { return vuy_; }
    double weight() const             { return wt_; }
    Rgb render_color() const          { return render_color_; }
    Eigen::Vector2d aim_pt() const    { return aim_pt_; }
    Eigen::Vector3d object_pt() const { return object_pt_; }

    void set_x(double x) { x_ = x; }
    void set_y(double y) { y_ = y; }
    void set_vlx(double vlx) { vlx_ = vlx; }
    void set_vux(double vux) { vux_ = vux; }
    void set_vly(double vly) { vly_ = vly; }
    void set_vuy(double vuy) { vuy_ = vuy; }
    void set_weight(double wt) { wt_= wt; }
    void set_render_color(const Rgb& color) { render_color_ = color; }
    void set_aim_pt(const Eigen::Vector2d& aim_pt) { aim_pt_ = aim_pt; }
    void set_object_pt(const Eigen::Vector3d& obj_pt) { object_pt_ = obj_pt; }

    Eigen::Vector2d apply_vignetting(const Eigen::Vector2d& pupil) const;



private:
    double x_;
    double y_;
    double wt_;
    double vux_;
    double vuy_;
    double vlx_;
    double vly_;
    Eigen::Vector2d aim_pt_;
    Eigen::Vector3d object_pt_;
    Rgb render_color_;
};


} //namespace geopter

#endif // FIELD_H

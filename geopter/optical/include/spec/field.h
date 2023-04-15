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

    double X() const                  { return x_; }
    double Y() const                  { return y_; }

    /** Vignetting factor of lower x */
    double VLX() const                { return vlx_; }

    /** Vignetting factor of upper x */
    double VUX() const                { return vux_; }

    /** Vignetting factor of lower y */
    double VLY() const                { return vly_; }

    /** Vignetting factor of upper y */
    double VUY() const                { return vuy_; }

    double Weight() const             { return wt_; }
    const Rgb& RenderColor() const          { return render_color_; }
    const Eigen::Vector2d& AimPt() const    { return aim_pt_; }
    const Eigen::Vector3d& ObjectPt() const { return object_pt_; }

    void SetX(double x) { x_ = x; }
    void SetY(double y) { y_ = y; }
    void SetVLX(double vlx) { vlx_ = vlx; }
    void SetVUX(double vux) { vux_ = vux; }
    void SetVLY(double vly) { vly_ = vly; }
    void SetVUY(double vuy) { vuy_ = vuy; }
    void SetWeight(double wt) { wt_= wt; }
    void SetRenderColor(const Rgb& color) { render_color_ = color; }
    void SetAimPt(const Eigen::Vector2d& aim_pt) { aim_pt_ = aim_pt; }
    void SetObjectPt(const Eigen::Vector3d& obj_pt) { object_pt_ = obj_pt; }

    Eigen::Vector2d ApplyVignetting(const Eigen::Vector2d& pupil) const;



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

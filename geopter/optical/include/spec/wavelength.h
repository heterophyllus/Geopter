/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi All Rights Reserved.
** 
** This file is part of Geopter.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
** 
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi                                   
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com                          
**             Date: May 16th, 2021                                                                                          
********************************************************************************/

#ifndef WAVELENGTH_H
#define WAVELENGTH_H

#include "spectral_line.h"
#include "renderer/rgb.h"

namespace geopter{

/** Wavelength component class */
class Wavelength
{
public:
    Wavelength(){
        value_ = SpectralLine::d;
        weight_ = 0.0;
        render_color_ = rgb_black;
    }

    Wavelength(double wl=SpectralLine::d, double wt=1.0, Rgb render_color=rgb_black) :
        value_(wl),
        weight_(wt),
        render_color_(render_color){}

    /**
     * @brief Set wavelength value
     * @param val_nm value in nm
     */
    void SetValue(double val_nm) { value_ = val_nm;}

    /** Returns wavelength value in nm */
    double Value() const {return value_;}

    void SetWeight(double wt) { weight_ = wt;}

    /** Returns the weight */
    double Weight() const { return weight_;}

    void SetRenderColor(const Rgb& color) { render_color_ = color;}

    /** Returns render color */
    const Rgb& RenderColor() const {return render_color_;}

private:
    /** wavelength in nm */
    double value_;

    /** wavelength weight used for MTF calculation, optimization, etc */
    double weight_;

    /** Render color used to draw ray line, plot ray fans, etc */
    Rgb render_color_;

};



} //namespace geopter

#endif

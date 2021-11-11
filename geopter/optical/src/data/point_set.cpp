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

//============================================================================
/// \file   point_set.cpp
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#include <iostream>
#include <iomanip>

#include "data/point_set.h"

using namespace geopter;

PointSet::PointSet() :
    render_color_(rgb_black),
    line_style_(1),
    line_width_(1.0)
{
    xdata_.clear();
    ydata_.clear();
}

PointSet::PointSet(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color, int line_style, double line_width) :
    xdata_(x),
    ydata_(y),
    render_color_(color),
    line_style_(line_style),
    line_width_(line_width)
{

}

PointSet::~PointSet()
{
    xdata_.clear();
    ydata_.clear();
}

void PointSet::print() const
{
    std::ostringstream oss;
    print(oss);
    std::cout << oss.str() << std::endl;
}

void PointSet::print(std::ostringstream &oss) const
{
    constexpr int idx_w = 8;
    constexpr int val_w = 12;
    constexpr int prec  = 4;

    oss << std::setw(idx_w) << std::right << "";
    oss << std::setw(idx_w) << std::right << name_;
    oss << std::endl;

    const int num_data = xdata_.size();
    for( int i = 0; i < num_data; i++) {
        oss << std::setw(idx_w) << std::right << std::fixed << std::setprecision(prec) << xdata_[i];
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << ydata_[i];
        oss << std::endl;
    }
}

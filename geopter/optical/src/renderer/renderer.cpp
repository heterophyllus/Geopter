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

#include "renderer/renderer.h"

using namespace geopter;

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::update()
{

}

void Renderer::draw_plot(const std::shared_ptr<PlotData> plotdata)
{

}

void Renderer::draw_text(std::string str, const Rgb& color, double line_width)
{

}

void Renderer::draw_line(const Eigen::Vector2d& p1, const Eigen::Vector2d& p2, const Rgb& color, int line_style, double line_width)
{

}

void Renderer::draw_polyline(const std::vector<Eigen::Vector2d>& pts, const Rgb& color, int line_style, double line_width)
{

}

void Renderer::draw_polyline(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color, int line_style, double line_width)
{

}

void Renderer::draw_dots(const std::vector<double> &x, const std::vector<double> &y, const Rgb &color, double dot_size)
{

}

void Renderer::draw_x_axis(bool state)
{

}

void Renderer::draw_y_axis(bool state)
{

}

void Renderer::set_x_axis_range(double xmin, double xmax)
{

}

void Renderer::set_y_axis_range(double ymin, double ymax)
{

}


void Renderer::set_x_axis_label(std::string label)
{

}

void Renderer::set_y_axis_label(std::string label)
{

}

void Renderer::set_aspect_ratio(double h_per_v)
{

}

void Renderer::set_grid_layout(int rows, int cols)
{

}

void Renderer::set_current_cell(int row, int col)
{

}

void Renderer::clear()
{

}

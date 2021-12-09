/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
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




#ifndef RENDERER_MPL_H
#define RENDERER_MPL_H

#include "renderer/renderer.h"

namespace geopter {

class RendererMpl : public Renderer
{
public:
    RendererMpl();
    ~RendererMpl();

    void set_grid_layout(int rows, int cols) override;
    void set_current_cell(int row, int col) override;

    void draw_line(const Eigen::Vector2d& p1, const Eigen::Vector2d& p2, const Rgb& color, int line_style=0, double line_width= 1.0) override;
    void draw_polyline(const std::vector<Eigen::Vector2d>& pts, const Rgb& color, int line_style=0, double line_width= 1.0) override;
    void draw_polyline(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color, int line_style=0, double line_width= 1.0) override;
    void draw_text(std::string str, const Rgb& color, double line_width= 1.0) override;
    void draw_dots(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color, double dot_size) override;

    void set_x_axis_range(double xmin, double xmax) override;
    void set_y_axis_range(double ymin, double ymax) override;
    void set_x_axis_label(std::string label) override;
    void set_y_axis_label(std::string label) override;

};

} // namespace geopter

#endif //RENDERER_MPL_H

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



#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <vector>

#include "Eigen/Core"
#include "renderer/rgb.h"

#include "data/plot_data.h"

namespace geopter {


class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    enum LineStyle
    {
        Solid,
        Dots
    };

    enum PlotStyle{
        Curve,
        Scatter
    };

    virtual void set_grid_layout(int rows, int cols);
    virtual void set_current_cell(int row, int col);

    virtual void draw_plot(const std::shared_ptr<PlotData> plotdata);

    virtual void draw_line(const Eigen::Vector2d& p1, const Eigen::Vector2d& p2, const Rgb& color, int line_style=0, double line_width= 1.0);
    virtual void draw_polyline(const std::vector<Eigen::Vector2d>& pts, const Rgb& color, int line_style=0, double line_width= 1.0);
    virtual void draw_polyline(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color, int line_style=0, double line_width= 1.0);
    virtual void draw_text(std::string str, const Rgb& color, double line_width= 1.0);
    virtual void draw_dots(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color, double dot_size);

    virtual void draw_x_axis(bool state= true);
    virtual void draw_y_axis(bool state= true);
    virtual void set_x_axis_range(double xmin, double xmax);
    virtual void set_y_axis_range(double ymin, double ymax);
    virtual void set_x_axis_label(std::string label);
    virtual void set_y_axis_label(std::string label);


    /**
     * @brief Set aspect ratio h:v
     * @param ratio aspect ratio h/v
     */
    virtual void set_aspect_ratio(double h_per_v);

    virtual void update();

    virtual void clear();

protected:
    int nrows_;
    int ncols_;
    int current_cell_index_;
    int current_line_width_;
    int current_line_style_;
};

} //namespace geopter

#endif // RENDERER_H

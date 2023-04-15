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
    Renderer(){};
    virtual ~Renderer(){};

    enum LineStyle
    {
        Solid,
        Dots
    };

    enum PlotStyle{
        Curve,
        Scatter
    };

    virtual void SetGridLayout(int rows, int cols){};
    virtual void SetCurrentCell(int row, int col){};

    virtual void DrawPlot(const std::shared_ptr<PlotData> plotdata){};

    virtual void DrawLine(const Eigen::Vector2d& p1, const Eigen::Vector2d& p2, const Rgb& color, int line_style=0, double line_width= 1.0){};
    virtual void DrawPolyline(const std::vector<Eigen::Vector2d>& pts, const Rgb& color, int line_style=0, double line_width= 1.0){};
    virtual void DrawPolyline(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color, int line_style=0, double line_width= 1.0){};
    virtual void DrawText(std::string str, const Rgb& color, double line_width= 1.0){};
    virtual void DrawDots(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color, double dot_size){};

    virtual void DrawXaxis(bool state= true){};
    virtual void DrawYaxis(bool state= true){};
    virtual void SetXaxisRange(double xmin, double xmax){};
    virtual void SetYaxisRange(double ymin, double ymax){};
    virtual void SetXaxisLabel(std::string label){};
    virtual void SetYaxisLabel(std::string label){};


    /**
     * @brief Set aspect ratio h:v
     * @param ratio aspect ratio h/v
     */
    virtual void SetAspectRatio(double h_per_v){};

    virtual void Update(){};

    virtual void Clear(){};

protected:
    int nrows_;
    int ncols_;
    int current_cell_index_;
    int current_line_width_;
    int current_line_style_;
};

} //namespace geopter

#endif // RENDERER_H

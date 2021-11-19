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

#ifndef GEOPTER_PLOT_DATA_3d_H
#define GEOPTER_PLOT_DATA_3d_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>

#include "Eigen/Core"
#include "renderer/rgb.h"
#include "data/grid_array.h"

namespace geopter{

struct Point3d
{
    Point3d(double xx=0.0, double yy=0.0, double zz=0.0){
        x = xx;
        y = yy;
        z = zz;
    }
    double x;
    double y;
    double z;
};

/** mainly used 3d colored map and its top view */
class MapData3d
{
public:
    MapData3d();
    ~MapData3d();

    void set_data(const std::vector<double>& x, const std::vector<double>& y, const GridArray<double>& z);

    void set_size(int rows, int cols, double fillin_val);

    Point3d cell(int i, int j);

    int rows() const;
    int cols() const;

    const std::vector<double>& xdata() {return xdata_;};
    const std::vector<double>& ydata() {return ydata_;};

    Eigen::MatrixXd to_matrix();

private:
    std::vector<double> xdata_;
    std::vector<double> ydata_;
    GridArray<double> zdata_;

    // RgbGradient colormap_;
};


}

#endif

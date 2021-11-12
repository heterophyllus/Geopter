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

#include "renderer/rgb.h"
#include "data/grid_array.h"

namespace geopter{

struct Point3d
{
    Point3d(double xx, double yy, double zz){
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

    void set_data(const GridArray<Point3d>& points);
    void set_data(const std::vector<double>& x, const std::vector<double>& y, const GridArray<double>& z);

    void set_cell_data(int i, int j, Point3d pt);
    void set_cell_data(int i, int j, double x, double y, double z);
    void set_size(int rows, int cols, double fillin_val);

    Point3d cell(int i, int j);

    int rows() const;
    int cols() const;

    void clear();

private:
    GridArray<Point3d> data_;

    int row_col_to_index(int row, int col);
    // RgbGradient colormap_;
};


}

#endif

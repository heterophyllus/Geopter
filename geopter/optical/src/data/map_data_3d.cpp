#include <limits>
#include <cmath>
#include "data/map_data_3d.h"

using namespace geopter;

MapData3d::MapData3d()
{

}

MapData3d::~MapData3d()
{

}

void MapData3d::clear()
{
    for(auto &r : data_){
        r.clear();
    }
    data_.clear();
}


void MapData3d::set_size(int rows, int cols, double fillin_val)
{
    this->clear();

    auto p = Point3d(fillin_val, fillin_val, fillin_val);

    for(int i = 0 ; i < rows; i++){
        std::vector<Point3d> grid_row(cols, p);
        data_.push_back(grid_row);
    }
}


void MapData3d::set_data(const GridArray<Point3d>& points)
{
    data_ = points;
}

void MapData3d::set_data(const std::vector<double> &x, const std::vector<double> &y, const GridArray<double> &z)
{
    assert(x.size() == y.size());
    int rows = y.size();
    int cols = x.size();

    set_size(rows, cols, NAN);

    for(int row = 0; row < rows; row++){
        for(int col = 0; col < cols; col++){
            int i = row_col_to_index(row, col);
            set_cell_data(row, col, Point3d(x[i], y[i], z[row][col]));
        }
    }
}

void MapData3d::set_cell_data(int i, int j, Point3d pt)
{
    data_[i][j] = pt;
}

void MapData3d::set_cell_data(int i, int j, double x, double y, double z)
{
    data_[i][j] = Point3d(x,y,z);
}

Point3d MapData3d::cell(int i, int j)
{
    return data_[i][j];
}

int MapData3d::rows() const
{
    return data_.size();
}

int MapData3d::cols() const
{
    return data_[0].size();
}

int MapData3d::row_col_to_index(int row, int col)
{
    //int rows = data_.size();
    int cols = data_[0].size();

    return cols*row + col;
}

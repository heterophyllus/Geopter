#include <limits>
#include <cmath>
#include "data/map_data_3d.h"

using namespace geopter;

MapData3d::MapData3d()
{

}

MapData3d::~MapData3d()
{
    xdata_.clear();
    ydata_.clear();

}


void MapData3d::set_size(int rows, int cols, double fillin_val)
{
    xdata_.resize(cols);
    ydata_.resize(rows);
    zdata_.resize(rows, cols);
    zdata_.fill(fillin_val);
}



void MapData3d::set_data(const std::vector<double> &x, const std::vector<double> &y, const GridArray<double> &z)
{
    assert(x.size() == y.size());

    xdata_ = x;
    ydata_ = y;
    zdata_ = z;
}


Point3d MapData3d::cell(int i, int j)
{
    return Point3d(xdata_[j], ydata_[i], zdata_.at(i, j));
}

int MapData3d::rows() const
{
    return ydata_.size();
}

int MapData3d::cols() const
{
    return xdata_.size();
}

Eigen::MatrixXd MapData3d::to_matrix()
{
    int rows = this->rows();
    int cols = this->cols();

    Eigen::MatrixXd m(rows, cols);

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            m(i,j) = zdata_.at(i, j);
        }
    }

    return m;
}

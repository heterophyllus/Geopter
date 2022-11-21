#include "data/data_grid.h"

using namespace geopter;

DataGrid::DataGrid(int nx, int ny, double dx, double dy) :
    nx_(nx), ny_(ny), dx_(dx), dy_(dy)
{
    value_data_ = Eigen::MatrixXd::Zero(ny_, nx_);
}

DataGrid::~DataGrid()
{

}


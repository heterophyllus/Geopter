#ifndef GEOPTER_DATA_GRID_H
#define GEOPTER_DATA_GRID_H

#include <string>

#include "Eigen/Core"

namespace geopter {

class DataGrid
{
public:
    double x(int row);
    double y(int col);
    double z(int row, int col);

    std::string description() const { return description_;}
    std::string x_label() const { return x_label_;}
    std::string y_label() const { return y_label_;}
    std::string value_label() const { return value_label_;}

    int Nx() const { return value_data_.rows();}
    int Ny() const { return value_data_.cols();}
    double dx() const { return dx_;}
    double dy() const { return dy_;}

    Eigen::MatrixXd value_data();

private:
    std::string description_;
    std::string x_label_;
    std::string y_label_;
    std::string value_label_;
    Eigen::MatrixXd value_data_;
    double dx_;
    double dy_;
};


}


#endif //GEOPTER_DATA_GRID_H

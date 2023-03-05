#ifndef GEOPTER_DATA_GRID_H
#define GEOPTER_DATA_GRID_H

#include <string>
#include <vector>

#include "Eigen/Core"

namespace geopter {

class DataGrid
{
public:
    DataGrid(int nx, int ny, double dx, double dy);
    ~DataGrid();
    
    double x(int col) { return dx_*(double)(col/nx_); }
    double y(int row) { return dy_*(double)(row/ny_); }
    double z(int row, int col) { return value_data_(row, col); }
    
    Eigen::MatrixXd& value_data() { return value_data_; }

    std::string description() const { return description_;}
    std::string x_label() const { return x_label_;}
    std::string y_label() const { return y_label_;}
    std::string value_label() const { return value_label_;}

    int nx() const { return nx_;}
    int ny() const { return ny_;}
    double dx() const { return dx_;}
    double dy() const { return dy_;}

    void set_description(const std::string& str){ description_ = str; }
    void set_x_label(const std::string& label) { x_label_ = label; }
    void set_y_label(const std::string& label) { y_label_ = label; }
    void set_value_label(const std::string& label) { value_label_ = label; }

    double get_value_at(int i, int j) { return value_data_(i, j); }
    void set_value_at(int i, int j, double val){ value_data_(i,j) = val; }

    void set_value_matrix(Eigen::MatrixXd& mat){ value_data_ = mat;}

private:
    std::string description_;
    std::string x_label_;
    std::string y_label_;
    std::string value_label_;
    Eigen::MatrixXd value_data_;
    int nx_;
    int ny_;
    double dx_;
    double dy_;
};


}


#endif //GEOPTER_DATA_GRID_H

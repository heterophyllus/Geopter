#ifndef GEOPTER_DATA_GRID_H
#define GEOPTER_DATA_GRID_H

#include <string>
#include <vector>

#include "Eigen/Core"

namespace geopter {

class DataGrid
{
public:
    DataGrid(int nx, int ny, double dx, double dy)
        :nx_(nx), ny_(ny), dx_(dx), dy_(dy)
    {
        value_data_ = Eigen::MatrixXd::Zero(ny_, nx_);
    }
    ~DataGrid(){};
    
    double X(int col) { return dx_*(double)(col/nx_); }
    double Y(int row) { return dy_*(double)(row/ny_); }
    double Z(int row, int col) { return value_data_(row, col); }
    
    Eigen::MatrixXd& ValueData() { return value_data_; }

    std::string Description() const { return description_;}
    std::string XLabel() const { return x_label_;}
    std::string YLabel() const { return y_label_;}
    std::string ValueLabel() const { return value_label_;}

    int Nx() const { return nx_;}
    int Ny() const { return ny_;}
    double Dx() const { return dx_;}
    double Dy() const { return dy_;}

    void SetDescription(const std::string& str){ description_ = str; }
    void SetXLabel(const std::string& label) { x_label_ = label; }
    void SetYLabel(const std::string& label) { y_label_ = label; }
    void SetValueLabel(const std::string& label) { value_label_ = label; }

    double GetValueAt(int i, int j) { return value_data_(i, j); }
    void SetValueAt(int i, int j, double val){ value_data_(i,j) = val; }

    void SetValueMatrix(Eigen::MatrixXd& mat){ value_data_ = mat;}

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

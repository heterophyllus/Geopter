#ifndef GEOPTER_DATA_SERIES_H
#define GEOPTER_DATA_SERIES_H


#include <string>
#include <vector>

#include "Eigen/Core"

namespace geopter {

class DataSeries
{
public:
    DataSeries();

    std::string description() const { return description_; }
    int number_of_series() const { return series_labels_.size(); }
    std::string& x_label() { return x_label_; }
    Eigen::VectorXd& x_data() { return x_data_; }

    /** y labels */
    std::vector<std::string>& series_labels() { return series_labels_; }
    Eigen::MatrixXd& y_data() { return y_data_; }

    void set_description(const std::string description) { description_ = description; }
    void set_x_label(std::string x_label) { x_label_ = x_label; }


protected:
    std::string description_;
    std::string x_label_;
    std::vector< std::string > series_labels_;
    Eigen::VectorXd x_data_;
    Eigen::MatrixXd y_data_;
};


}



#endif //GEOPTER_DATA_SERIES_H

#include <cassert>
#include <iomanip>
#include <limits>
#include <algorithm>
#include "data/graph2d.h"

using namespace geopter;

Graph2d::Graph2d()
{
    name_ = "";
    render_color_ = rgb_black;
    line_width_ = 1.0;
    line_style_ = 0;
}

Graph2d::Graph2d(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color, int line_style, double line_width)
{
    render_color_ = color;
    line_width_ = line_width;
    line_style_ = line_style;
    x_data_ = x;
    y_data_ = y;
}

Graph2d::~Graph2d()
{
    x_data_.clear();
    y_data_.clear();
}

void Graph2d::set_data(const std::vector<double>& x, const std::vector<double>& y)
{
    assert(x.size() == y.size());

    x_data_ = x;
    y_data_ = y;
}

void Graph2d::add_data(double x, double y)
{
    x_data_.push_back(x);
    y_data_.push_back(y);
}

void Graph2d::get_data(std::vector<double>& xdata, std::vector<double>& ydata)
{
    xdata = x_data_;
    ydata = y_data_;
}

void Graph2d::get_x_range(double* lower, double* higher) const
{
    *lower  = *std::min_element(x_data_.begin(), y_data_.end());
    *higher = *std::max_element(x_data_.begin(), y_data_.end());
}

void Graph2d::get_y_range(double* lower, double* higher) const
{
    *lower  = *std::min_element(y_data_.begin(), y_data_.end());
    *higher = *std::max_element(y_data_.begin(), y_data_.end());
}

void Graph2d::print(std::ostringstream &oss)
{
    constexpr int idx_w = 8;
    constexpr int val_w = 12;
    constexpr int prec  = 4;

    oss << std::setw(idx_w) << std::right << "";
    oss << std::setw(idx_w) << std::right << name_;
    oss << std::endl;

    const int num_data = x_data_.size();
    for( int i = 0; i < num_data; i++) {
        oss << std::setw(idx_w) << std::right << std::fixed << std::setprecision(prec) << x_data_[i];
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << y_data_[i];
        oss << std::endl;
    }
}

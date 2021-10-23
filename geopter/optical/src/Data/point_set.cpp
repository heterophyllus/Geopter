#include <iostream>
#include <iomanip>

#include "Data/point_set.h"

using namespace geopter;

PointSet::PointSet() :
    render_color_(rgb_black),
    line_style_(1),
    line_width_(1.0)
{
    xdata_.clear();
    ydata_.clear();
}

PointSet::PointSet(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color, int line_style, double line_width) :
    xdata_(x),
    ydata_(y),
    render_color_(color),
    line_style_(line_style),
    line_width_(line_width)
{

}

PointSet::~PointSet()
{
    xdata_.clear();
    ydata_.clear();
}

void PointSet::print() const
{
    std::ostringstream oss;
    print(oss);
    std::cout << oss.str() << std::endl;
}

void PointSet::print(std::ostringstream &oss) const
{
    constexpr int idx_w = 8;
    constexpr int val_w = 12;
    constexpr int prec  = 4;

    oss << std::setw(idx_w) << std::right << "";
    oss << std::setw(idx_w) << std::right << name_;
    oss << std::endl;

    const int num_data = xdata_.size();
    for( int i = 0; i < num_data; i++) {
        oss << std::setw(idx_w) << std::right << std::fixed << std::setprecision(prec) << xdata_[i];
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << ydata_[i];
        oss << std::endl;
    }
}

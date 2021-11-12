#include <cassert>
#include <iomanip>
#include <limits>
#include "data/graph2d.h"

using namespace geopter;

Graph2d::Graph2d()
{
    name_ = "";
    render_color_ = rgb_black;
    line_width_ = 1.0;
    line_style_ = 0;
}

Graph2d::Graph2d(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color, double line_width, int line_style)
{
    render_color_ = color;
    line_width_ = line_width;
    line_style_ = line_style;

    this->set_data(x,y);
}

Graph2d::~Graph2d()
{
    data_.clear();
}

void Graph2d::set_data(const std::vector<double>& x, const std::vector<double>& y)
{
    assert(x.size() == y.size());

    const int num_pts = x.size();

    for(int i = 0; i < num_pts; i++){
        auto p = Point2d(x[i], y[i]);
        data_.emplace_back(p);
    }
    
}

void Graph2d::add_data(double x, double y)
{
    data_.emplace_back(Point2d(x, y));
}

void Graph2d::get_data(std::vector<double>& xdata, std::vector<double>& ydata)
{
    const int num_pts = data_.size();

    xdata.clear();
    ydata.clear();

    xdata.reserve(num_pts);
    ydata.reserve(num_pts);

    for(int i = 0; i < num_pts; i++){
        xdata.push_back(data_[i].x);
        ydata.push_back(data_[i].y);
    }

}

DataRange Graph2d::xrange() const
{
    double xmin = std::numeric_limits<double>::infinity();
    double xmax = - std::numeric_limits<double>::infinity();
    const int num_pts = data_.size();
    for(int i = 0; i < num_pts; i++) {
        if(data_[i].x < xmin){
            xmin = data_[i].x;
        }

        if(data_[i].x > xmax){
            xmax = data_[i].x;
        }
    }

    return DataRange(xmin, xmax);
}

DataRange Graph2d::yrange() const
{
    double ymin = std::numeric_limits<double>::infinity();
    double ymax = - std::numeric_limits<double>::infinity();
    const int num_pts = data_.size();
    for(int i = 0; i < num_pts; i++) {
        if(data_[i].y < ymin){
            ymin = data_[i].y;
        }

        if(data_[i].y > ymax){
            ymax = data_[i].y;
        }
    }

    return DataRange(ymin, ymax);
}

void Graph2d::print(std::ostringstream &oss)
{
    constexpr int idx_w = 8;
    constexpr int val_w = 12;
    constexpr int prec  = 4;

    oss << std::setw(idx_w) << std::right << "";
    oss << std::setw(idx_w) << std::right << name_;
    oss << std::endl;

    const int num_data = data_.size();
    for( int i = 0; i < num_data; i++) {
        oss << std::setw(idx_w) << std::right << std::fixed << std::setprecision(prec) << data_[i].x;
        oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << data_[i].y;
        oss << std::endl;
    }
}

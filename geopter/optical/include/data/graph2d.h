#ifndef GEOPTER_POINT_2D_H
#define GEOPTER_POINT_2D_H

#include <string>
#include <vector>
#include <sstream>
#include "renderer/rgb.h"

namespace geopter{

struct Point2d
{
    Point2d(double xx, double yy)
    {
        x = xx;
        y = yy;
    }
    double x;
    double y;
};

struct DataRange
{
    DataRange(double l, double h){
        lower = l;
        higher = h;
    }

    double higher;
    double lower;
};

class Graph2d
{
public:
    Graph2d();
    Graph2d(const std::vector<double>& x, const std::vector<double>& y, const Rgb& color= rgb_black, int line_style=0, double line_width=1.0);
    ~Graph2d();

    void set_data(const std::vector<double>& x, const std::vector<double>& y);
    void add_data(double x, double y);
    inline void set_name(const std::string& name);
    inline void set_render_color(const Rgb& color);
    inline void set_line_width(double lw);
    inline void set_line_style(int ls);

    inline int data_count() const;
    inline Point2d data(int i) const;
    inline std::string name() const;
    inline Rgb render_color() const;
    inline double line_width() const;
    inline int line_style() const;

    DataRange xrange() const;
    DataRange yrange() const;

    void get_data(std::vector<double>& xdata, std::vector<double>& ydata);

    void print(std::ostringstream& oss);

private:
    std::vector< Point2d > data_;
    std::string name_;
    Rgb render_color_;
    int line_style_;
    double line_width_;
};

void Graph2d::set_name(const std::string &name)
{
    name_ = name;
}

void Graph2d::set_render_color(const Rgb &color)
{
    render_color_ = color;
}

void Graph2d::set_line_width(double lw)
{
    line_width_ = lw;
}

void Graph2d::set_line_style(int ls)
{
    line_style_ = ls;
}

int Graph2d::data_count() const
{
    return data_.size();
}

Point2d Graph2d::data(int i) const
{
    return data_[i];
}

std::string Graph2d::name() const
{
    return name_;
}

Rgb Graph2d::render_color() const
{
    return render_color_;
}

double Graph2d::line_width() const
{
    return line_width_;
}

int Graph2d::line_style() const
{
    return line_style_;
}


} //namespace geopter

#endif 

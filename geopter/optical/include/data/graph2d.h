#ifndef GEOPTER_POINT_2D_H
#define GEOPTER_POINT_2D_H

#include <string>
#include <vector>
#include <sstream>
#include "renderer/rgb.h"

namespace geopter{

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
    inline void get_data(double* x, double* y) const;
    inline std::string name() const;
    inline Rgb render_color() const;
    inline double line_width() const;
    inline int line_style() const;

    void get_x_range(double* lower, double* higher) const;
    void get_y_range(double* lower, double* higher) const;

    void get_data(std::vector<double>& xdata, std::vector<double>& ydata);

    inline const std::vector<double>& x_data() const;
    inline const std::vector<double>& y_data() const;

    void print(std::ostringstream& oss);

private:
    std::vector<double> x_data_;
    std::vector<double> y_data_;
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
    return x_data_.size();
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

const std::vector<double>& Graph2d::x_data() const
{
    return x_data_;
}

const std::vector<double>& Graph2d::y_data() const
{
    return y_data_;
}

} //namespace geopter

#endif 

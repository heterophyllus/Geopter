#ifndef PLOTDATA_H
#define PLOTDATA_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Renderer/rgb.h"

namespace geopter {


class PointGroup
{
public:
    PointGroup();
    ~PointGroup();

    void set_data(const std::vector<double>& x, const std::vector<double>& y);
    void add_data(double x, double y);
    void set_render_color(const Rgb& color);
    void set_line_width(double lw);
    void set_line_style(int ls);

    std::vector<double>& x_data() const;
    std::vector<double>& y_data() const;
    Rgb render_color() const;
    int line_style() const;
    double line_width() const;

private:
    std::vector<double> xdata_;
    std::vector<double> ydata_;
    Rgb render_color_;
    int line_style_;
    double line_width_;
};


class PlotData
{
public:
    PlotData();
    ~PlotData();

    void add_point_group(std::shared_ptr<PointGroup> points);
    void add_optional_data(std::string dataname, double value);
    void set_name(std::string name);

    std::shared_ptr<PointGroup> point_group(int i) const;
    double get_optional_data(std::string dataname) const;
    std::string name() const;

private:
    std::vector< std::shared_ptr<PointGroup> > point_groups_;
    std::map<std::string, double> optional_data_;
    std::string name_;
};

}

#endif // PLOTDATA_H

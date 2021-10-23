#ifndef PLOT_DATA_H
#define PLOT_DATA_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>

#include "Data/point_set.h"

namespace geopter {

class PlotData
{
public:
    PlotData();
    ~PlotData();

    inline void add_pointset(std::shared_ptr<PointSet> points);
    inline void add_optional_data(const std::string& dataname, double value);
    inline void set_title(const std::string& title);
    inline void set_x_axis_label(const std::string& label);
    inline void set_y_axis_label(const std::string& label);

    inline int data_count() const;

    inline std::shared_ptr<PointSet> point_set(int i) const;
    inline double get_optional_data(const std::string& dataname) const;
    inline std::string title() const;
    inline std::string x_axis_label() const;
    inline std::string y_axis_label() const;

    void print(std::ostringstream& oss);
    void print();

private:
    std::vector< std::shared_ptr<PointSet> > pointsets_;
    std::map<std::string, double> optional_data_;
    std::string title_;
    std::string x_axis_label_;
    std::string y_axis_label_;
    bool xy_reverse_;
};


void PlotData::add_pointset(std::shared_ptr<PointSet> points)
{
    pointsets_.push_back(points);
}

void PlotData::add_optional_data(const std::string& dataname, double value)
{
    optional_data_.insert(std::make_pair(dataname, value));
}

void PlotData::set_title(const std::string &title)
{
    title_ = title;
}

void PlotData::set_x_axis_label(const std::string& label)
{
    x_axis_label_ = label;
}

void PlotData::set_y_axis_label(const std::string& label)
{
    y_axis_label_ = label;
}

std::shared_ptr<PointSet> PlotData::point_set(int i) const
{
    return pointsets_[i];
}

double PlotData::get_optional_data(const std::string& dataname) const
{
    return optional_data_.at(dataname);
}

std::string PlotData::title() const
{
    return title_;
}

int PlotData::data_count() const
{
    return pointsets_.size();
}

std::string PlotData::x_axis_label() const
{
    return x_axis_label_;
}

std::string PlotData::y_axis_label() const
{
    return y_axis_label_;
}

}

#endif // PLOTDATA_H

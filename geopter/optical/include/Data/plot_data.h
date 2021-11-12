/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
** 
** This file is part of Geopter.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
** 
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi                                   
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com                          
**             Date: May 16th, 2021                                                                                          
********************************************************************************/

#ifndef PLOT_DATA_H
#define PLOT_DATA_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>

#include "data/graph2d.h"

namespace geopter {

class PlotData
{
public:
    PlotData();
    ~PlotData();

    inline void add_graph(std::shared_ptr<Graph2d> graph);
    inline void add_optional_data(const std::string& dataname, double value);
    inline void set_title(const std::string& title);
    inline void set_x_axis_label(const std::string& label);
    inline void set_y_axis_label(const std::string& label);

    inline int data_count() const;

    inline std::shared_ptr<Graph2d> graph(int i) const;
    inline double get_optional_data(const std::string& dataname) const;
    inline std::string title() const;
    inline std::string x_axis_label() const;
    inline std::string y_axis_label() const;

    void print(std::ostringstream& oss);
    void print();

private:
    std::vector< std::shared_ptr<Graph2d> > graphs_;
    std::map<std::string, double> optional_data_;
    std::string title_;
    std::string x_axis_label_;
    std::string y_axis_label_;
    bool xy_reverse_;
};


void PlotData::add_graph(std::shared_ptr<Graph2d> graph)
{
    graphs_.push_back(graph);
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

std::shared_ptr<Graph2d> PlotData::graph(int i) const
{
    return graphs_[i];
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
    return graphs_.size();
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

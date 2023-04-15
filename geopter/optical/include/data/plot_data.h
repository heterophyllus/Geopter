/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
** 
** This file is part of Geopter.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public
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

    void AddGraph(std::shared_ptr<Graph2d> graph){ graphs_.push_back(graph); }
    void AddOptionalData(const std::string& dataname, double value) { optional_data_.insert(std::make_pair(dataname, value)); }
    void SetTitle(const std::string& title) { title_ = title; }
    void SetXLabel(const std::string& label) { x_axis_label_ = label;}
    void SetYLabel(const std::string& label) { y_axis_label_ = label; }
    void SetPlotStyle(int ps) { plot_style_ = ps; }
    void SetXYReverse(bool xy_reverse) { xy_reverse_ = xy_reverse; }

    int NumberOfGraphs() const { return graphs_.size(); }

    std::shared_ptr<Graph2d> GetGraph(int i) const { return graphs_[i]; }
    double GetOptionalData(const std::string& dataname) const { return optional_data_.at(dataname);}
    std::string title() const { return title_;}
    std::string XLabel() const { return x_axis_label_; }
    std::string YLabel() const { return y_axis_label_;}
    int PlotStyle() const { return plot_style_; }

    void Print(std::ostringstream& oss);
    void Print();

private:
    std::vector< std::shared_ptr<Graph2d> > graphs_;
    std::map<std::string, double> optional_data_;
    std::string title_;
    std::string x_axis_label_;
    std::string y_axis_label_;
    int plot_style_;
    bool xy_reverse_;
    bool common_x_label_;
};


}

#endif // PLOTDATA_H

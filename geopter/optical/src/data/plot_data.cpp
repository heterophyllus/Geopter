/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi All Rights Reserved.
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


#include <iostream>
#include <iomanip>

#include "data/plot_data.h"

using namespace geopter;


PlotData::PlotData() :
    title_(""),
    x_axis_label_("x"),
    y_axis_label_("y"),
    plot_style_(0),
    xy_reverse_(false)
{
    graphs_.clear();
    optional_data_.clear();
}


PlotData::~PlotData()
{
    for(auto &g: graphs_){
        g.reset();
    }
    graphs_.clear();

    optional_data_.clear();
}


void PlotData::print()
{
    std::ostringstream oss;
    print(oss);
    std::cout << oss.str() << std::endl;
}

void PlotData::print(std::ostringstream &oss)
{    
    constexpr int idx_w = 8;
    constexpr int val_w = 12;
    constexpr int prec  = 4;

    oss << std::setw(idx_w) << std::right << title_ << std::endl;
    oss << std::endl;

    if(xy_reverse_){
        // labels
        oss << std::setw(idx_w) << std::right << std::fixed << y_axis_label_;
        for(auto &g : graphs_){
            oss << std::setw(val_w) << std::right << std::fixed << g->name();
        }
        oss << std::endl;

        // data
        const int num_data = graphs_[0]->data_count();
        for(int i = 0; i < num_data; i++){
            oss << std::setw(idx_w) << std::right << std::fixed << std::setprecision(prec) << graphs_[0]->y_data()[i];
            for(auto &g : graphs_){
                oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << g->x_data()[i];
            }
            oss << std::endl;
        }

    }else{
        // labels
        oss << std::setw(idx_w) << std::right << std::fixed << x_axis_label_;
        for(auto &g : graphs_){
            oss << std::setw(val_w) << std::right << std::fixed << g->name();
        }
        oss << std::endl;

        // data
        const int num_data = graphs_[0]->data_count();
        for(int i = 0; i < num_data; i++){
            oss << std::setw(idx_w) << std::right << std::fixed << std::setprecision(prec) << graphs_[0]->x_data()[i];
            for(auto &g : graphs_){
                oss << std::setw(val_w) << std::right << std::fixed << std::setprecision(prec) << g->y_data()[i];
            }
            oss << std::endl;
        }
    }
    oss << std::endl;
}

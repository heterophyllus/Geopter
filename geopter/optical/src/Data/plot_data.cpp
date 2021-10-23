#include <iostream>
#include <iomanip>

#include "Data/plot_data.h"

using namespace geopter;



PlotData::PlotData() :
    title_(""),
    x_axis_label_("x"),
    y_axis_label_("y")
{
    pointsets_.clear();
    optional_data_.clear();
}


PlotData::~PlotData()
{
    for(auto &ps: pointsets_){
        ps.reset();
    }
    pointsets_.clear();

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
    oss << title_ << std::endl;
    oss << std::endl;

    // data
    for(auto &ps : pointsets_){
        ps->print(oss);
        oss << std::endl;
    }

}

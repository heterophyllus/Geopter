#include "data/spot_data.h"

using namespace geopter;

SpotData::SpotData() :
    ref_point_(PointData())
{

}

SpotData::~SpotData()
{
    points_.clear();
}

void SpotData::resize(int n)
{
    points_.resize(n);
}

double SpotData::max_radius() const
{
    const double x0 = ref_point_.x;
    const double y0 = ref_point_.y;

    double max_r2 = 0.0;
    for(auto &p : points_){
        double dx = p.x - x0;
        double dy = p.y - y0;
        double r2 = dx*dx + dy*dy;
        if(max_r2 < r2) max_r2 = r2;
    }

    return sqrt(max_r2);
}

double SpotData::rms_radius() const
{
    const double x0 = ref_point_.x;
    const double y0 = ref_point_.y;

    double sum_r2 = 0.0;
    for(auto &p : points_){
        double dx = p.x - x0;
        double dy = p.y - y0;
        double r2 = dx*dx + dy*dy;
        sum_r2 += r2;
    }
    const int n = points_.size();

    return sqrt(sum_r2/n);
}

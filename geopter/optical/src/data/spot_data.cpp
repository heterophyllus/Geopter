
#include "data/spot_data.h"

using namespace geopter;

SpotData::SpotData() :
    ref_x_(0.0), ref_y_(0.0)
{

}

SpotData::SpotData(int n) :
    ref_x_(0.0), ref_y_(0.0)
{
    this->allocate(n);
}

SpotData::~SpotData()
{
    segments_.clear();
}

double SpotData::rms_spot_x() const
{
    double x2_sum = 0.0;
    for(auto &seg : segments_){
        double x = seg.x() - ref_x_;
        x2_sum += x*x;
    }

    int n = segments_.size();

    return sqrt( x2_sum/n );
}

double SpotData::rms_spot_y() const
{
    double y2_sum = 0.0;
    for(auto &seg : segments_){
        double y = seg.y() - ref_y_;
        y2_sum += y*y;
    }

    int n = segments_.size();

    return sqrt( y2_sum/n );
}

double SpotData::rms_spot_size() const
{
    double h2_sum = 0.0;
    for(auto &seg : segments_){
        double x  = seg.x() - ref_x_;
        double y  = seg.y() - ref_y_;
        double h2 = x*x + y*y;
        h2_sum += h2;
    }

    int n = segments_.size();

    return sqrt( h2_sum/n );
}

double SpotData::geo_spot_size() const
{
    double h2_max = 0.0;
    for(auto &seg : segments_){
        double x  = seg.x() - ref_x_;
        double y  = seg.y() - ref_y_;
        double h2 = x*x + y*y;

        if(h2_max < h2) h2_max = h2;
    }

    return sqrt(h2_max);
}

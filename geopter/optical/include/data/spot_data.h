#ifndef GEOPTER_SPOT_DATA_H
#define GEOPTER_SPOT_DATA_H

#include <vector>

#include "sequential/ray_segment.h"

namespace geopter {

class SpotData
{
public:
    SpotData();
    SpotData(int n);
    ~SpotData();

    RaySegment& at(int i) { return segments_[i];}
    double ref_x() const { return ref_x_;}
    double ref_y() const { return ref_y_;}
    double rms_spot_size() const;
    double rms_spot_x() const;
    double rms_spot_y() const;
    double geo_spot_size() const;

    void allocate(int n) { segments_.resize(n);}
    void set_ref_x(double x) { ref_x_ = x;}
    void set_ref_y(double y) { ref_y_ = y;}
    void set_segment(int i, const RaySegment& segment) { segments_[i] = segment; }

private:
    double ref_x_;
    double ref_y_;
    std::vector<RaySegment> segments_;
};


}

#endif //GEOPTER_SPOT_DATA_H

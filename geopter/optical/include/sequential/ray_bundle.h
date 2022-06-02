#ifndef RAY_BUNDLE_H
#define RAY_BUNDLE_H

#include "Eigen/Core"
#include "data/spot_data.h"

namespace geopter{


class RayBundle
{
public:
    RayBundle();
    ~RayBundle();

    PointData& at(int n) {return spot_data_.at(n);}
    void resize(int n){spot_data_.resize(n);}
    void create_pattern();
    int num_rays() const {return spot_data_.number_of_points();}

private:
    int row_col_to_index(int row, int col);
    SpotData spot_data_;
    Eigen::Vector3d obj_coord_;
};

}



#endif



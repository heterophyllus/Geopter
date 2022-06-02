#ifndef GEOPTER_SPOT_DATA_H
#define GEOPTER_SPOT_DATA_H

#include <vector>
#include "Eigen/Core"

namespace geopter{

struct PointData
{
    PointData(double xx= 0.0, double yy= 0.0, double zz=0.0, double op=0.0){
        x = xx;
        y = yy;
        z = zz;
        opl = op;
    }

    // ray intersection point at image
    double x;
    double y;
    double z;

    // total optical path length
    double opl;
};


class SpotData
{
public:
    SpotData();
    ~SpotData();

    void resize(int n);

    int number_of_points() const {return points_.size();}

    PointData &at(int i){return points_[i];}

    /** Calculates geometric spot radius */
    double max_radius() const;

    /** Calculates RMS spot radius */
    double rms_radius() const;

    void set_obj_coord(const Eigen::Vector3d& obj_coord) {obj_coord_ = obj_coord; }

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

private:
    std::vector<PointData> points_;
    PointData ref_point_; // chief ray intersection point at image
    Eigen::Vector3d obj_coord_;
};


}


#endif //GEOPTER_SPOT_DATA_H

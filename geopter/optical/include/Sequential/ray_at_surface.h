#ifndef RAY_AT_SURFACE_H
#define RAY_AT_SURFACE_H

#include "Eigen/Core"

namespace geopter {

/** Ray parameters on the surface */
class RayAtSurface
{
public:
    RayAtSurface(){
        intersect_pt_ = Eigen::Vector3d::Zero(3);
        normal_       = Eigen::Vector3d::Zero(3);
        after_dir_    = Eigen::Vector3d::Zero(3);
        distance_from_before_ = 0.0;
        optical_path_length_  = 0.0;
        before_ = nullptr;
    }

    RayAtSurface(const Eigen::Vector3d& inc_pt, const Eigen::Vector3d& normal, const Eigen::Vector3d& after_dir, double dist, double opl, RayAtSurface *before=nullptr){
        intersect_pt_ = inc_pt;
        normal_ = normal;
        after_dir_ = after_dir;
        distance_from_before_ = dist;
        optical_path_length_ = opl;
        before_ = before;
    }

    ~RayAtSurface(){
        before_ = nullptr;
    }

    void set_before(RayAtSurface* before){
        before_ = before;
    }

    inline Eigen::Vector3d intersect_pt() const;

    inline double distance_from_before() const;

    /** Local coordinate at the intersection point */
    inline double x() const;
    inline double y() const;
    inline double z() const;

    inline Eigen::Vector3d after_dir() const;

    inline double L() const;
    inline double M() const;
    inline double N() const;

    inline Eigen::Vector3d surface_normal() const;

    inline double srl() const;
    inline double srm() const;
    inline double srn() const;

    /** Ray height from the axis */
    inline double height() const;

    /** Angle of incidence (signed) */
    inline double aoi() const;

    /** Angle of refraction (signed) */
    inline double aor() const;


private:
    /** ray intersect point on the surface */
    Eigen::Vector3d intersect_pt_;

    /** surface normal at the intersect point */
    Eigen::Vector3d normal_;

    /** directional cosine after surface interaction */
    Eigen::Vector3d after_dir_;

    /** distance from the previous point to the intersect point */
    double distance_from_before_;

    /** optical path length from the previous to the current */
    double optical_path_length_;

    RayAtSurface* before_;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};




Eigen::Vector3d RayAtSurface::intersect_pt() const
{
    return intersect_pt_;
}

double RayAtSurface::x() const
{
    return intersect_pt_(0);
}

double RayAtSurface::y() const
{
    return intersect_pt_(1);
}

double RayAtSurface::z() const
{
    return intersect_pt_(2);
}

double RayAtSurface::height() const
{
    return sqrt( pow(intersect_pt_(0),2) + pow(intersect_pt_(1),2) );
}

double RayAtSurface::distance_from_before() const
{
    return distance_from_before_;
}

Eigen::Vector3d RayAtSurface::after_dir() const
{
    return after_dir_;
}

double RayAtSurface::L() const
{
    return after_dir_(0);
}

double RayAtSurface::M() const
{
    return after_dir_(1);
}

double RayAtSurface::N() const
{
    return after_dir_(2);
}

Eigen::Vector3d RayAtSurface::surface_normal() const
{
    return normal_;
}

double RayAtSurface::srl() const
{
    return normal_(0);
}

double RayAtSurface::srm() const
{
    return normal_(1);
}

double RayAtSurface::srn() const
{
    return normal_(2);
}

double RayAtSurface::aoi() const
{
    Eigen::Vector3d inc_dir;

    if(before_) {
        inc_dir = before_->after_dir();
    }else{
        inc_dir = after_dir_;
    }

    // We need signed value
    double tanU1 = inc_dir(1)/inc_dir(2);
    double tanU2 = normal_(1)/normal_(2);
    double tanI = (tanU1 - tanU2)/(1.0 + tanU1*tanU2); // I = U1-U2
    return atan(tanI);
}

double RayAtSurface::aor() const
{
    double tanU1 = after_dir_(1)/after_dir_(2);
    double tanU2 = normal_(1)/normal_(2);
    double tanI_prime = (tanU1 - tanU2)/(1.0 + tanU1*tanU2);
    return atan(tanI_prime);
}

}

#endif

#ifndef RAY_H
#define RAY_H

#include <vector>
#include <sstream>
#include <memory>

#include "Eigen/Core"


namespace geopter {


/** Ray parameters on each surface */
struct RayAtSurface
{
    RayAtSurface(){
        intersect_pt = Eigen::Vector3d::Zero(3);
        normal       = Eigen::Vector3d::Zero(3);
        after_dir    = Eigen::Vector3d::Zero(3);
        distance_from_before = 0.0;
        optical_path_length  = 0.0;
        before = nullptr;
    }

    /** ray intersect point on the surface */
    Eigen::Vector3d intersect_pt;

    /** surface normal at the intersect point */
    Eigen::Vector3d normal;

    /** directional cosine after surface interaction */
    Eigen::Vector3d after_dir;

    /** distance from the previous point to the intersect point */
    double distance_from_before;

    /** optical path length from the previous to the current */
    double optical_path_length;

    RayAtSurface* before;

    // aliases
    double x() const { return intersect_pt(0); }
    double y() const { return intersect_pt(1); }
    double z() const { return intersect_pt(2); }

    /** Ray height from the axis */
    double height() const { return sqrt( pow(intersect_pt(0),2) + pow(intersect_pt(1),2) ); }
};


enum RayStatus
{
    PassThrough,
    Blocked,
    MissedSurface,
    TotalReflection,
};


class Ray
{
public:
    Ray();
    ~Ray();

    int size() const;

    void clear();

    /** Add data at the beginning */
    void prepend(std::shared_ptr<RayAtSurface> ray_at_srf);

    /** Add data at the last */
    void append(std::shared_ptr<RayAtSurface> ray_at_srf);

    RayAtSurface* at(int i) const;
    RayAtSurface* front() const;
    RayAtSurface* back() const;

    void set_wvl(double wvl);

    int status() const;
    void set_status(int s);

    /* aliases */
    /** x intersect coordinate at the given surface */
    double x(int i) const;

    /** y intersect coordinate at the given surface */
    double y(int i) const;

    /** z intersect coordinate at the given surface */
    double z(int i) const;

    /** direcional cosine after the given surface */
    double L(int i) const;
    double M(int i) const;
    double N(int i) const;

    /** angle of incidence at the given surface in radian */
    double aoi(int i) const;

    /** angle of refraction at the given surface in radian */
    double aor(int i);

    double srl(int i);
    double srm(int i);
    double srn(int i);

    void print(std::ostringstream& oss);
    void print();

private:
    std::vector< std::shared_ptr<RayAtSurface> > ray_at_srfs_;
    double wvl_;
    int status_;
};

} //namespace geopter

#endif // RAY_H

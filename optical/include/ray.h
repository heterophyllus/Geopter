#ifndef RAY_H
#define RAY_H

#include <vector>
#include <iostream>
#include "Eigen/Core"


namespace geopter {


/**
 * Component of Ray class.
 * Ray interaction data at the surface.
 */
struct RayAtSurface
{
    /** ray intersect point on the surface */
    Eigen::Vector3d intersect_pt;

    /** surface norm at the intersect point */
    Eigen::Vector3d normal;

    /** directional cosine after surface interaction */
    Eigen::Vector3d after_dir;


    double after_dist;

    /** optical path length from the previous to the current */
    double optical_path;
};


enum RayStatus
{
    PassThrough,
    Blocked,
    MissedSurface,
    TotalReflection,
};

/** This class describes ray property as a result of ray tracing */
class Ray
{
public:
    Ray();
    ~Ray();

    void clear();

    void append(RayAtSurface ray_at_srf);

    void set_op_delta(double op_delta);
    void set_wvl(double wvl);

    int size() const;

    RayAtSurface at(int i) const;

    /** Return data at the last surface (at image in most cases) */
    RayAtSurface last() const;

    void set_status(int s);
    int status() const;

    void print(std::ostringstream& oss);
    void print();

private:
    std::vector<RayAtSurface> ray_at_srfs_;
    double wvl_;
    double op_delta_;
    int status_;
};

} //namespace geopter

#endif // RAY_H

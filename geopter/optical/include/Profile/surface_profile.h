#ifndef SURFACEPROFILE_H
#define SURFACEPROFILE_H


#include <vector>
#include "Eigen/Core"

namespace geopter {

enum SurfaceType
{
    Sphere,
    EvenAsphere,
    OddAsphere
};

class SurfaceProfile
{
public:
    SurfaceProfile();
    virtual ~SurfaceProfile();

    virtual std::string name() const;

    virtual void set_cv(double c);

    /** Returns center curvature */
    virtual double cv() const;

    virtual void set_radius(double r);

    /** Returns center radius */
    virtual double radius() const;

    /** Returns the value of the profile function at point @param{p} */
    virtual double f(const Eigen::Vector3d& p) const;

    /** Returns the gradient of the profile function at point *p* */
    virtual Eigen::Vector3d df(const Eigen::Vector3d& p) const;

    /** Returns the unit normal of the profile at point *p* */
    virtual Eigen::Vector3d normal(const Eigen::Vector3d& p) const;

    /** Returns the sagitta (z coordinate) of the surface at x, y */
    virtual double sag(double x, double y) const;

    virtual double deriv_1st(double h) const;
    virtual double deriv_2nd(double h) const;


    /**
     * @brief Intersect a profile, starting from an arbitrary point
     * @param p0 start point of the ray in the profile's coordinate system
     * @param d direction cosine of the ray in the profile's coordinate system
     * @param eps numeric tolerance for convergence of any iterative procedure
     * @param z_dir +1 if propagation positive direction, -1 if otherwise
     */
    virtual void intersect(Eigen::Vector3d& pt, double& s, const Eigen::Vector3d& p0, const Eigen::Vector3d& d, double eps=1.0e-12, double z_dir=1.0);

    /**
     * @brief Intersect a profile, starting from an arbitrary point
     * @note From Spencer and Murty, General Ray-Tracing Procedure <https://doi.org/10.1364/JOSA.52.000672>
     * @param pt intersect point
     * @param distance to the intersect point
     * @param p0 start point of the ray in the profile's coordinate system
     * @param d direction cosine of the ray in the profile's coordinate system
     * @param eps numeric tolerance for convergence of any iterative procedure
     * @param z_dir +1 if propagation positive direction, -1 if otherwise
     */
    virtual void intersect_spencer(Eigen::Vector3d& pt, double& s, const Eigen::Vector3d& p0, const Eigen::Vector3d& d, double eps=1.0e-12, double z_dir=1.0);


protected:
    double cv_;
    std::string name_;
};

} //namespace geopter

#endif // SURFACEPROFILE_H

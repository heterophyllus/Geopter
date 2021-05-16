#ifndef SURFACEPROFILE_H
#define SURFACEPROFILE_H


#include <vector>
#include "Eigen/Core"

namespace geopter {

enum ProfileType
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

    virtual void update();

    virtual std::string name() const;

    virtual void set_cv(double c);
    virtual double cv() const;

    virtual void set_radius(double r);
    virtual double radius() const;

    /** Returns the value of the profile function at point @param{p} */
    virtual double f(Eigen::Vector3d p) const;

    /** Returns the gradient of the profile function at point *p* */
    virtual Eigen::Vector3d df(Eigen::Vector3d p) const;

    /** Returns the unit normal of the profile at point *p* */
    virtual Eigen::Vector3d normal(Eigen::Vector3d p) const;

    /** Returns the sagitta (z coordinate) of the surface at x, y */
    virtual double sag(double x, double y) const;

    /** Return a 2d polyline approximating the surface profile
     *  @param sd semi-diameter of the profile
     *  @param dir +1 for profile from neg to postive direction, -1 if otherwise
     *  @param steps number of points to generate
     */
    //virtual Profile profile(SemiDiameter sd, int dir=1, int steps=6){};
    //virtual void profile(std::vector<Vector2d>& prf, SemiDiameter sd, int dir=1, int steps=6){};

    /**
     * @brief Intersect a profile, starting from an arbitrary point
     * @param p0 start point of the ray in the profile's coordinate system
     * @param d direction cosine of the ray in the profile's coordinate system
     * @param eps numeric tolerance for convergence of any iterative procedure
     * @param z_dir +1 if propagation positive direction, -1 if otherwise
     */
    //virtual IntersectPointAndDistance intersect(Vector3d p0, Vector3d d, double eps=1.0e-12, int z_dir=1.0);

    virtual void intersect(Eigen::Vector3d& pt, double& s, Eigen::Vector3d p0, Eigen::Vector3d d, double eps=1.0e-12, int z_dir=1);

    /**
     * @brief Intersect a profile, starting from an arbitrary point
     * @note From Spencer and Murty, General Ray-Tracing Procedure <https://doi.org/10.1364/JOSA.52.000672>
     * @param p0 start point of the ray in the profile's coordinate system
     * @param d direction cosine of the ray in the profile's coordinate system
     * @param eps numeric tolerance for convergence of any iterative procedure
     * @param z_dir +1 if propagation positive direction, -1 if otherwise
     */
    //virtual IntersectPointAndDistance intersect_spencer(Vector3d p0, Vector3d d, double eps=1.0e-12, int z_dir=1.0);

    virtual void intersect_spencer(Eigen::Vector3d& pt, double& s, Eigen::Vector3d p0, Eigen::Vector3d d, double eps=1.0e-12, int z_dir=1.0);


protected:
    /** center curvature */
    double cv_;
    std::string name_;
};

} //namespace geopter

#endif // SURFACEPROFILE_H

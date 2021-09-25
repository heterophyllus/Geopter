#ifndef SURFACE_H
#define SURFACE_H

#include "Assembly/interface.h"

namespace geopter {

/** Optical refractive surface */
class Surface : public Interface
{
public:
    Surface(std::string lbl="");
    Surface(double r);
    ~Surface();

    void update() override;

    /**
     * @brief Compute intersect point and distance from the previous interface
     * @param pt intersect point
     * @param s distance from the previous interface
     * @param p0
     * @param d
     * @param eps precision
     * @param z_dir
     */
    void intersect(Eigen::Vector3d& pt, double& s, const Eigen::Vector3d& p0, const Eigen::Vector3d& d, double eps=1.0e-12, double z_dir=1.0);

    Eigen::Vector3d normal(Eigen::Vector3d p);

    void print();
    void print(std::ostringstream& oss);

protected:


};

} //namespace geopter

#endif // SURFACE_H

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

    /** Returns optical power */
    //double optical_power() const;

    //void set_optical_power(double pwr, double n_before, double n_after) override;
    //void set_optical_power(double pwr);

    //void get_y_aperture_extent(double& max_aperture, double& min_aperture);

    /**
     * @brief Compute intersect point and distance from the previous interface
     * @param pt intersect point
     * @param s distance from the previous interface
     * @param p0
     * @param d
     * @param eps precision
     * @param z_dir
     */
    void intersect(Eigen::Vector3d& pt, double& s, Eigen::Vector3d p0, Eigen::Vector3d d, double eps=1.0e-12, double z_dir=1.0);

    Eigen::Vector3d normal(Eigen::Vector3d p);

    void print();
    void print(std::ostringstream& oss);

protected:
    //double optical_power_;


};

} //namespace geopter

#endif // SURFACE_H

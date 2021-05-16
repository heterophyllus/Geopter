#ifndef SURFACE_H
#define SURFACE_H

#include "interface.h"

namespace geopter {


/** Optical refractive surface */
class Surface : public Interface
{

public:
    Surface(std::string lbl="");
    ~Surface();

    void update() override;

    double optical_power();
    void set_optical_power(double pwr, double n_before, double n_after) override;
    void set_optical_power(double pwr);

    void get_y_aperture_extent(double& max_aperture, double& min_aperture);

    void intersect(Eigen::Vector3d& pt, double& s, Eigen::Vector3d p0, Eigen::Vector3d d, double eps=1.0e-12, double z_dir=1.0);

    Eigen::Vector3d normal(Eigen::Vector3d p);

    void print();
    void print(std::ostringstream& oss);

protected:
    double optical_power_;


};

} //namespace geopter

#endif // SURFACE_H

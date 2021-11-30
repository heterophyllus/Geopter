#ifndef GEOMETRICAL_MTF_H
#define GEOMETRICAL_MTF_H

#include "data/plot_data.h"
#include "system/optical_system.h"

namespace geopter{

class GeometricalMTF
{
public:
    GeometricalMTF();

    std::shared_ptr<PlotData> plot(OpticalSystem* opt_sys, int nrd, double max_freq= 100.0, double freq_step= 5.0);

private:
    double calc_geo_mtf(double s, double t, const std::vector<double>& u, const std::vector<double>& v);
};

}

#endif

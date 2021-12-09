#ifndef GEOPTER_DIFFRACTIVE_MTF_H
#define GEOPTER_DIFFRACTIVE_MTF_H

#include "analysis/wave_aberration.h"

namespace geopter{

class DiffractiveMTF : WaveAberration
{
public:
    DiffractiveMTF(OpticalSystem *opt_sys);

    std::shared_ptr<PlotData> plot(OpticalSystem* opt_sys, int M, double L=1.0);

    Eigen::MatrixXd to_matrix();

protected:

};

}

#endif //GEOPTER_DIFFRACTIVE_MTF_H

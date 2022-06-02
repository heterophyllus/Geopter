#ifndef GEOPTER_DIFFRACTIVE_MTF_H
#define GEOPTER_DIFFRACTIVE_MTF_H

#include "analysis/wave_aberration.h"

namespace geopter{

class DiffractiveMTF : WaveAberration
{
public:
    DiffractiveMTF(OpticalSystem *opt_sys);

    std::shared_ptr<PlotData> plot(OpticalSystem* opt_sys, int M);

protected:

};

}

#endif //GEOPTER_DIFFRACTIVE_MTF_H

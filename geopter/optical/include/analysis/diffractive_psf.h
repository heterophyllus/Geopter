#ifndef GEOPTER_DIFFRACTIVE_PSF_H
#define GEOPTER_DIFFRACTIVE_PSF_H

#include "analysis/wavefront.h"


namespace geopter{

class DiffractivePSF : WaveAberration
{
public:
    DiffractivePSF(OpticalSystem *opt_sys);

    std::shared_ptr<DataGrid> Create(const Field* fld, double wvl, int ndim);

    void CreateFromOpdTrace(OpticalSystem* opt_sys, const Field* fld, double wvl, int M, double L=1.0);

    void CreateFromSpotData();

    Eigen::MatrixXd &ConvertToMatrix();

protected:
    int ndim_;
    Eigen::MatrixXd W_;
    Eigen::MatrixXcd coh_;
    Eigen::MatrixXd psf_;
};

}

#endif //GEOPTER_DIFFRACTIVE_PSF_H

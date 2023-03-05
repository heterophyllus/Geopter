#ifndef GEOPTER_DIFFRACTIVE_PSF_H
#define GEOPTER_DIFFRACTIVE_PSF_H

#include "analysis/wavefront.h"


namespace geopter{

class DiffractivePSF : WaveAberration
{
public:
    DiffractivePSF(OpticalSystem *opt_sys);

    std::shared_ptr<DataGrid> create(const Field* fld, double wvl, int ndim);

    void from_opd_trace(OpticalSystem* opt_sys, const Field* fld, double wvl, int M, double L=1.0);

    void create_from_spot_data();

    Eigen::MatrixXd &to_matrix();

protected:
    int ndim_;
    Eigen::MatrixXd W_;
    Eigen::MatrixXcd coh_;
    Eigen::MatrixXd psf_;
};

}

#endif //GEOPTER_DIFFRACTIVE_PSF_H

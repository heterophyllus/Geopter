#ifndef GEOPTER_DIFFRACTIVE_PSF_H
#define GEOPTER_DIFFRACTIVE_PSF_H

#include "analysis/wavefront.h"
#include "data/map_data_3d.h"

namespace geopter{

class DiffractivePSF : WaveAberration
{
public:
    DiffractivePSF(OpticalSystem *opt_sys);

    std::shared_ptr<MapData3d> plot(const Field* fld, double wvl, int ndim, int maxdim);

    Eigen::MatrixXd calc_psf(const Eigen::MatrixXd& wavefront, int ndim, int maxdim);

    Eigen::MatrixXcd fft2(Eigen::MatrixXcd& mat);

    GridArray<double> matrix_to_grid_array(const Eigen::MatrixXd& mat);
};

}

#endif //GEOPTER_DIFFRACTIVE_PSF_H

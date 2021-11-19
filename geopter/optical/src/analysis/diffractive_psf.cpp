#include <complex>
#include <iostream>
#include <fstream>
#include "unsupported/Eigen/MatrixFunctions"
#include "unsupported/Eigen/FFT"
#include "analysis/diffractive_psf.h"
#include "analysis/circ_shift.h"

using namespace geopter;

DiffractivePSF::DiffractivePSF(OpticalSystem *opt_sys) :
    WaveAberration(opt_sys)
{

}

std::shared_ptr<MapData3d> DiffractivePSF::plot(const Field* fld, double wvl, int ndim, int maxdim)
{
    Wavefront *wf = new Wavefront(opt_sys_);
    auto wf_mapdata = wf->plot(fld, wvl, ndim);

    Eigen::MatrixXd wavefront = wf_mapdata->to_matrix();

    Eigen::MatrixXd psf = calc_psf(wavefront, ndim, maxdim);

    GridArray<double> psf_grid = matrix_to_grid_array(psf);

    auto psf_mapdata = std::make_shared<MapData3d>();

    double r = opt_sys_->fundamental_data().exp_radius;
    double D = 2.0*r;
    double du = D/ndim;
    int N = psf.rows();
    std::vector<double> x(N), y(N);
    for(int i = 0; i < N; i++) {
        x[i] = -2.0*r + du*(double)i/(double)(N-1);
    }
    y = x;

    psf_mapdata->set_data(x, y, psf_grid);

    return psf_mapdata;
}

Eigen::MatrixXd DiffractivePSF::calc_psf(const Eigen::MatrixXd& wavefront, int ndim, int maxdim)
{

    int maxdim_by_2 = maxdim/2;
    Eigen::MatrixXcd W = Eigen::MatrixXcd::Zero(maxdim, maxdim);
    int nd2 = ndim/2;


    int ii = -1;
    int jj = -1;
    for(int i = maxdim_by_2-(nd2-1)-1; i < maxdim_by_2+(nd2+1)-1; i++){
        ii++;
        jj = -1;
        for(int j = maxdim_by_2-(nd2-1)-1; j < maxdim_by_2+(nd2+1)-1; j++) {
            jj++;
            if(std::isnan(wavefront(ii,jj))){
                W(i,j) = 0.0;
            }else{
                W(i,j) = wavefront(ii,jj);
            }
        }
    }

    //Eigen::MatrixXcd phase_exp = 1i * 2.0 * M_PI * W;
    Eigen::MatrixXcd phase_exp = std::complex<double>(0.0,2.0*M_PI)*W;
    Eigen::MatrixXcd phase = phase_exp.array().exp();

    int phase_rows = phase.rows();
    int phase_cols = phase.cols();

    for(int i = 0; i < phase_rows; i++) {
        for(int j = 0; j < phase_cols; j++) {
            if(norm(phase(i, j) - 1.0) < std::numeric_limits<double>::epsilon()) {
                phase(i, j) = 0.0;
            }
        }
    }

    Eigen::MatrixXcd AP1 = fftshift(phase);
    Eigen::MatrixXcd AP2 = fft2(AP1);
    Eigen::MatrixXcd AP3 = fftshift(AP2);
    Eigen::MatrixXd AP = AP3.cwiseAbs2();

    double AP_max = AP.maxCoeff();
    //double AP_min = AP.minCoeff();

    AP = AP/AP_max;

    AP.transposeInPlace();
    return AP;
}

Eigen::MatrixXcd DiffractivePSF::fft2(Eigen::MatrixXcd& mat)
{
    Eigen::FFT<double> fft;

    int rows = mat.rows();
    int cols = mat.cols();

    Eigen::MatrixXcd temp(rows, cols);
    Eigen::MatrixXcd result(rows, cols);

    for(int j = 0; j < cols; j++) {
        temp.col(j) = fft.fwd(mat.col(j));
    }

    temp.transposeInPlace();

    for(int j = 0; j < cols; j++) {
        result.col(j) = fft.fwd(temp.col(j));
    }

    return result;
}

GridArray<double> DiffractivePSF::matrix_to_grid_array(const Eigen::MatrixXd &mat)
{
    int rows = mat.rows();
    int cols = mat.cols();
    GridArray<double> grid(rows, cols);

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            grid.set_cell(i, j, mat(i,j));
        }
    }

    return grid;
}

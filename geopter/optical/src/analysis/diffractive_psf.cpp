#define _USE_MATH_DEFINES
#include <cmath>
#include <complex>
#include <iostream>
#include <fstream>
#include "Eigen/Dense"
#include "unsupported/Eigen/MatrixFunctions"
#include "unsupported/Eigen/FFT"
#include "analysis/diffractive_psf.h"
#include "sequential/sequential_trace.h"
#include "sequential/trace_error.h"
#include "common/matrix_tool.h"
#include "common/circ_shift.h"

using namespace geopter;

DiffractivePSF::DiffractivePSF(OpticalSystem *opt_sys) :
    WaveAberration(opt_sys)
{

}
Eigen::MatrixXd &DiffractivePSF::ConvertToMatrix()
{
    return psf_;
}


std::shared_ptr<DataGrid> DiffractivePSF::Create(const Field *fld, double wvl, int ndim)
{
    WavefrontMap *wfm = new WavefrontMap(opt_sys_);

    auto wf_grid = wfm->Create(fld,wvl,ndim);

    Eigen::MatrixXd W = wf_grid->ValueData();

    for(int i = 0; i < ndim; i++){
        for(int j = 0; j < ndim; j++){
            if(std::isnan(W(i,j))){
                W(i,j) = 0.0;
            }
        }
    }

    Eigen::MatrixXd O = Eigen::MatrixXd::Zero(2*ndim, 2*ndim);

    O.block(ndim/2, ndim/2, ndim, ndim) = W;

    std::complex<double> im(0.0, 1.0);
    double lambda = wvl*1.0e-6;
    //double k = 2.0*M_PI/lambda;
    double k = M_PI;
    Eigen::MatrixXcd H = O.array() * ((-k*im*O).array().exp());
    //Eigen::MatrixXcd H = O;
    Eigen::MatrixXd psf = ((fftshift( MatrixTool::fft2(ifftshift(H)).matrix() )).array().abs() ).block(ndim/2, ndim/2, ndim, ndim) ;
    //Eigen::MatrixXd psf = ((fftshift( MatrixTool::fft2(ifftshift(O)).matrix() )).array().abs() ).block(ndim/2, ndim/2, ndim, ndim);

    psf /= psf.array().maxCoeff();

    auto psf_grid = std::make_shared<DataGrid>(2*ndim, 2*ndim, 1.0, 1.0);
    psf_grid->SetValueMatrix(psf);

    return psf_grid;

}

void DiffractivePSF::CreateFromOpdTrace(OpticalSystem* opt_sys, const Field* fld, double wvl, int M, double L)
{
    /*
     * David G. Voelz, "Computational fourier optics : a MATLAB tutorial", SPIE
     *
     */

    SequentialTrace *tracer = new SequentialTrace(opt_sys);
    tracer->SetApertureCheck(true);
    tracer->SetApplyVig(false);

    SequentialPath seq_path = tracer->CreateSequentialPath(wvl);

    auto chief_ray = std::make_shared<Ray>();
    chief_ray->Allocate(seq_path.Size());
    if( TRACE_SUCCESS != tracer->TracePupilRay(chief_ray, seq_path, Eigen::Vector2d({0.0, 0.0}), fld, wvl) ){
        std::cerr << "Trace error" << std::endl;
    }

    double du = L/static_cast<double>(M);
    double img_ht = chief_ray->GetBack()->Height();
    double img_dist = opt_sys->GetFirstOrderData()->image_distance;
    double exp_dist = opt_sys->GetFirstOrderData()->exit_pupil_distance;
    double zxp = img_dist - exp_dist;
    double dxp = sqrt(zxp*zxp + img_ht*img_ht);
    double wxp = opt_sys->GetFirstOrderData()->exit_pupil_radius;
    double lambda = wvl*1.0e-6;
    double lz = lambda*dxp;
    double k = 2.0*M_PI/lambda;

    std::vector<double> u(M), v(M);
    std::vector<double> fu(M), fv(M);

    for(int i = 0; i < M; i++){
        u[i] = -L/2.0 + static_cast<double>(i)*du;
        fu[i]= -1.0/(2.0*du) + static_cast<double>(i)*(1.0/L);
    }
    fv = fu;



    W_ = Eigen::MatrixXd::Zero(M, M);
    Eigen::MatrixXcd A = Eigen::MatrixXcd::Zero(M, M);
    Eigen::Vector2d pupil;
    RayPtr ray = std::make_shared<Ray>(seq_path.Size());

    for(int i = 0; i < M; i++){
        for(int j = 0; j < M; j++){
            pupil(0) = fu[j] * lz/wxp;
            pupil(1) = fv[i] * lz/wxp;

            if(pupil.norm() <= 1.0){
                tracer->TracePupilRay(ray, seq_path, pupil, fld, wvl);

                if(ray->Status() == TRACE_SUCCESS){
                    double opd = wave_abr_full_calc(ray, chief_ray);
                    W_(i,j) = opd;
                    A(i,j) = 1.0;
                }else{
                    W_(i,j) = 0.0;
                    A(i,j) = 0.0;
                }
            }else{
                W_(i,j) = 0.0;
                A(i,j) = 0.0;
            }

        }
    }

    delete tracer;

    std::complex<double> im(0.0, 1.0);

    Eigen::MatrixXcd H = A.array() * ((-k*im*W_).array().exp());
    //coh_ = H;

    //Eigen::MatrixXcd psf1 = ifftshift(H);
    //Eigen::MatrixXcd psf2 = MatrixTool::fft2(psf1);
    //Eigen::MatrixXcd psf2 = MatrixTool::fft2(ifftshift(H));
    //Eigen::MatrixXcd psf3 = fftshift(psf2);
    //Eigen::MatrixXcd psf3 = fftshift( MatrixTool::fft2(ifftshift(H)).matrix() );
    //psf_ = psf3.array().abs();
    psf_ = (fftshift( MatrixTool::fft2(ifftshift(H)).matrix() )).array().abs();

}



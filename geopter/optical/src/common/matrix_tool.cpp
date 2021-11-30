#include "common/matrix_tool.h"
#include "unsupported/Eigen/FFT"

using namespace geopter;

void MatrixTool::fft2(Eigen::MatrixXcd& out, const Eigen::MatrixXcd& in)
{
    Eigen::FFT<double> fft;

    int rows = in.rows();
    int cols = in.cols();

    Eigen::MatrixXcd temp(rows, cols);

    for (int k = 0; k < rows; k++) {
        Eigen::VectorXcd tv(in.row(k));
        temp.row(k) = fft.fwd(tv).transpose();
    }

    for (int k = 0; k < cols; k++) {
        Eigen::VectorXcd tv(temp.col(k));
        out.col(k) = fft.fwd(tv);
    }

}

void MatrixTool::ifft2(Eigen::MatrixXcd& out, const Eigen::MatrixXcd& in)
{
    int rows = in.rows();
    int cols = in.cols();
    Eigen::MatrixXcd mat_c = in.conjugate();
    MatrixTool::fft2(out, mat_c);
    out = out.conjugate()/(rows*cols);
}

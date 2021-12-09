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

Eigen::MatrixXcd MatrixTool::fft2(const Eigen::MatrixXcd& in)
{
    Eigen::FFT<double> fft;

    int rows = in.rows();
    int cols = in.cols();

    Eigen::MatrixXcd temp(rows, cols);
    Eigen::MatrixXcd result(rows, cols);

    // https://github.com/Daniele122898/NumericalScience2019/blob/a16c6fa9a744df0abb718e1f7b4ab565730191a9/exercises4/template_fft2.cpp
    for (int k = 0; k < rows; k++) {
        Eigen::VectorXcd tv(in.row(k));
        temp.row(k) = fft.fwd(tv).transpose();
    }

    for (int k = 0; k < cols; k++) {
        Eigen::VectorXcd tv(temp.col(k));
        result.col(k) = fft.fwd(tv);
    }

    return result;
}

Eigen::MatrixXcd MatrixTool::ifft2(const Eigen::MatrixXcd& mat)
{
    int rows = mat.rows();
    int cols = mat.cols();
    Eigen::MatrixXcd mat_c = mat.conjugate();
    Eigen::MatrixXcd C = fft2(mat_c);
    Eigen::MatrixXcd result = C.conjugate()/(rows*cols);

    return result;
}

std::vector<double> MatrixTool::to_std_vector(const Eigen::VectorXd& v)
{
    std::vector<double> v_std(&v[0], v.data()+v.cols()*v.rows());
    return v_std;
}

Eigen::VectorXd MatrixTool::from_std_vector(const std::vector<double> &v)
{
    std::vector<double> vv = v;
    Eigen::VectorXd v_eig = Eigen::Map<Eigen::VectorXd>(vv.data(), vv.size());
    return v_eig;
}

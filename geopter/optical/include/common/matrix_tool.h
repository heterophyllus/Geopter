#ifndef GROPTER_MATRIX_TOOL_H
#define GROPTER_MATRIX_TOOL_H

#include <fstream>
#include "Eigen/Core"
#include "Eigen/Dense"

namespace geopter {

class MatrixTool
{
public:
    static void fft2(Eigen::MatrixXcd& out, const Eigen::MatrixXcd& in);
    static void ifft2(Eigen::MatrixXcd& out, const Eigen::MatrixXcd& in);
    static Eigen::MatrixXcd fft2(const Eigen::MatrixXcd& in);
    static Eigen::MatrixXcd ifft2(const Eigen::MatrixXcd& in);

    static std::vector<double> to_std_vector(const Eigen::VectorXd& v);

    static Eigen::VectorXd from_std_vector(const std::vector<double>& v);

    template <typename XprType>
    static void csvwrite(const std::string& filepath, Eigen::DenseBase<XprType>& mat);
};



template <typename XprType>
void MatrixTool::csvwrite(const std::string& filepath, Eigen::DenseBase<XprType>& mat)
{
    std::fstream fout(filepath, std::ios::out);

    int rows = mat.rows();
    int cols = mat.cols();

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++) {
            fout << mat(i,j) << ", ";
        }
        fout << std::endl;
    }

    fout.close();
}


}

#endif //GROPTER_MATRIX_TOOL_H

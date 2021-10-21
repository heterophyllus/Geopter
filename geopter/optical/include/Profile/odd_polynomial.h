#ifndef ODDPOLYNOMIAL_H
#define ODDPOLYNOMIAL_H

#include <vector>

#include "surface_profile.h"

namespace geopter {

class OddPolynomial : public SurfaceProfile
{
public:
    OddPolynomial(double cv = 0.0);
    OddPolynomial(double cv, double k, const std::vector<double>& coefs);
    ~OddPolynomial();

    /** Returns the conic factor */
    double conic() const;

    /** Returns aspherical coefficient at the specified index */
    double coef(int i) const;

    int coef_count() const;
    double sag(double x, double y) const override;
    double f(const Eigen::Vector3d& p) const override;
    Eigen::Vector3d df(const Eigen::Vector3d& p) const override;
    double deriv_1st(double h) const override;
    double deriv_2nd(double h) const override;

    void set_conic(double k);
    void set_coef(int i, double val);
    void set_coef(const std::vector<double>& coefs);

private:
    void update_max_nonzero_index();

    double conic_;
    int max_nonzero_index_;
    std::vector<double> coefs_;
    const int num_coefs_;
};

}

#endif //ODDPOLYNOMIAL_H

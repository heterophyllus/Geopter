#ifndef EVENPOLYNOMIAL_H
#define EVENPOLYNOMIAL_H

#include <vector>

#include "surface_profile.h"


namespace geopter {

/** Even polynomial aspherical shape */
class EvenPolynomial : public SurfaceProfile
{
public:
    EvenPolynomial(double cv= 0.0);
    EvenPolynomial(double cv, double conic, const std::vector<double>& coefs);
    ~EvenPolynomial();

    /** Returns the conic factor */
    double conic() const;

    /** Returns aspherical coefficient at the specified index */
    double coef(int i) const;

    /** Returns number of coefficients */
    int coef_count() const;

    double sag(double x, double y) const override;
    double f(const Eigen::Vector3d& p) const override;
    Eigen::Vector3d df(const Eigen::Vector3d& p) const override;
    double deriv_1st(double h) const override;
    double deriv_2nd(double h) const override;

    void set_conic(double cc);
    void set_coef(int i, double val);
    void set_coef(const std::vector<double>& coefs);

private:
    void update_max_nonzero_index();

    double conic_;
    int max_nonzero_index_;
    std::vector<double> coefs_;
    const int num_coefs_;
};

} //namespace

#endif // EVENPOLYNOMIAL_H

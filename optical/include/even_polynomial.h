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

    void set_conic(double cc);

    /** Conic constant */
    double conic() const;

    void set_coef(int i, double val);
    void set_coef(const std::vector<double>& coefs);

    /** Returns aspherical coefficient at the specified index */
    double coef(int i) const;

    int coef_count() const;

    double sag(double x, double y) const override;
    double f(Eigen::Vector3d p) const override;
    Eigen::Vector3d df(Eigen::Vector3d p) const override;

private:
    double conic_;
    int max_nonzero_index_;
    std::vector<double> coefs_;
    const int num_coefs_;

    void update_max_nonzero_index();
};

} //namespace

#endif // EVENPOLYNOMIAL_H

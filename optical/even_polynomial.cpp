#include "even_polynomial.h"
#include "trace_error.h"

using namespace geopter;

EvenPolynomial::EvenPolynomial(double cv) :
    conic_(0.0),
    max_nonzero_index_(0),
    num_coefs_(10)
{
    name_ = "ASP";
    cv_ = cv;
    coefs_ = std::vector<double>(num_coefs_, 0.0);
}

EvenPolynomial::EvenPolynomial(double cv, double conic, const std::vector<double>& coefs) :
    conic_(conic),
    max_nonzero_index_(0),
    num_coefs_(10)
{
    name_ = "ASP";
    cv_ = cv;
    coefs_ = std::vector<double>(num_coefs_, 0.0);
    this->set_coef(coefs);
}

EvenPolynomial::~EvenPolynomial()
{
    coefs_.clear();
}

void EvenPolynomial::set_conic(double cc)
{
    conic_ = cc;
}

double EvenPolynomial::conic() const
{
    return conic_;
}

double EvenPolynomial::coef(int i) const
{
    if(i < num_coefs_){
        return coefs_[i];
    }else{
        return 0.0;
    }
}

void EvenPolynomial::set_coef(int i, double val)
{
    if(i < num_coefs_){
        coefs_[i] = val;
        update_max_nonzero_index();
    }
}

void EvenPolynomial::set_coef(const std::vector<double>& coefs)
{
    coefs_ = std::vector<double>(num_coefs_, 0.0);

    for(int i = 0; i < std::min(num_coefs_, (int)coefs.size()); i++){
        coefs_[i] = coefs[i];
    }

    update_max_nonzero_index();
}

int EvenPolynomial::coef_count() const
{
    return coefs_.size();
}

double EvenPolynomial::sag(double x, double y) const
{
    double r2 = x*x + y*y;

    // sphere + conic contribution
    double z = 0.0;
    try {
        z = cv_*r2 / ( 1.0 + sqrt(1.0 - (conic_+1.0)*cv_*cv_*r2) );
    }  catch (...) {
        throw TraceMissedSurfaceError();
    }

    // polynomial contribution
    double z_asp = 0.0;
    double r_pow = r2;

    r_pow *= r2;

    for(int i = 0; i < num_coefs_; i++){
        z_asp += coefs_[i]*r_pow;
        r_pow *= r2;
    }

    return (z + z_asp);
}

double EvenPolynomial::f(Eigen::Vector3d p) const
{
    return ( p(2) - this->sag(p(0), p(1)) );
}

Eigen::Vector3d EvenPolynomial::df(Eigen::Vector3d p) const
{
    //sphere + conic contribution
    double r2 = p(0)*p(0) + p(1)*p(1);
    double ec = conic_ + 1.0;
    double e = cv_ / sqrt( 1.0 - ec*cv_*cv_*r2 );

    //polynomial asphere contribution
    double r_pow = 1.0;
    double e_asp = 0.0;
    double c_coef = 2.0;

    c_coef += 2.0;
    r_pow *= r2;

    for(int i = 0; i < num_coefs_; i++){
        e_asp += c_coef*coefs_[i]*r_pow;
        c_coef += 2.0;
        r_pow *= r2;
    }

    double e_tot = e + e_asp;

    Eigen::Vector3d df({-e_tot*p(0), -e_tot*p(1), 1.0});
    return df;
}

void EvenPolynomial::update_max_nonzero_index()
{
    double eps = 1.0e-20;
    int i = 0;
    while(i < num_coefs_){
        if(abs(coefs_[i]-0.0) < eps){
            max_nonzero_index_ = i;
            break;
        }
        i++;
    }
}

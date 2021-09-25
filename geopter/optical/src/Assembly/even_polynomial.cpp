#include "Assembly/even_polynomial.h"

#include "Sequential/trace_error.h"

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

double EvenPolynomial::f(const Eigen::Vector3d& p) const
{
    return ( p(2) - this->sag(p(0), p(1)) );
}

Eigen::Vector3d EvenPolynomial::df(const Eigen::Vector3d& p) const
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

double EvenPolynomial::deriv_1st(double h) const
{
    double k = conic_;
    double z_sqrt = sqrt(1.0 - cv_*cv_*h*h*(k+1));

    double z1 = 2*cv_*h/ ( 1.0 + z_sqrt );

    double z2_denom = pow(cv_,3)*pow(h,3)*(k+1);
    double z2_num = z_sqrt*pow(1.0 + z_sqrt, 2);
    double z2 = z2_denom/z2_num;

    double z3 = 0.0;
    for (int i = 0; i < num_coefs_; i++) {
        z3 += 2*(i+2) * coefs_[i] * pow(h, 2*(i+1) + 1);
    }

    return (z1 + z2 + z3);
}

double EvenPolynomial::deriv_2nd(double h) const
{
    double k = conic_;
    double z_sqrt = sqrt(1.0 - cv_*cv_*h*h*(k+1));

    double z1 = 2*cv_ / (1.0 + z_sqrt);

    double z2_denom = 5*pow(cv_,3)*pow(h,2)*(k+1);
    double z2_num = z_sqrt*pow(1.0 + z_sqrt, 2);
    double z2 = z2_denom/z2_num;

    double z3_denom = pow(cv_,5)*pow(h,4)*pow(k+1, 2);
    double z3_num = pow(z_sqrt, 3) * pow(z_sqrt + 1.0 , 2);
    double z3 = z3_denom/z3_num;

    double z4_denom = -2*pow(cv_,5)*pow(h,4)*pow(k+1, 2);
    double z4_num = ( cv_*cv_*h*h*(k+1) - 1.0 ) * pow( 1.0 + z_sqrt , 3);
    double z4 = z4_denom/z4_num;

    double z5 = 0.0;
    for(int i = 0; i < num_coefs_; i++) {
        double numeric_coef = ( 2*(i+1) + 1 ) * ( 2*(i+1) + 2 );
        //double z5_comp = numeric_coef * coefs_[i] * pow(h, 2*(i+1));
        z5 += numeric_coef * coefs_[i] * pow(h, 2*(i+1));
    }

    return (z1 + z2 + z3 + z4 + z5);
}

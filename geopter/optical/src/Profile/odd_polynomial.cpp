#include "Profile/odd_polynomial.h"
#include "Sequential/trace_error.h"

using namespace geopter;

OddPolynomial::OddPolynomial(double cv) :
    conic_(0.0),
    max_nonzero_index_(0),
    num_coefs_(10)
{
    name_ = "ODD";
    cv_ = cv;
    coefs_ = std::vector<double>(num_coefs_, 0.0);
}

OddPolynomial::OddPolynomial(double cv, double conic, const std::vector<double>& coefs) :
    conic_(conic),
    max_nonzero_index_(0),
    num_coefs_(10)
{
    name_ = "ODD";
    cv_ = cv;
    this->set_coef(coefs);
}

OddPolynomial::~OddPolynomial()
{
    coefs_.clear();
}

void OddPolynomial::set_conic(double k)
{
    conic_ = k;
}

void OddPolynomial::set_coef(int i, double val)
{
    if(i < num_coefs_){
        coefs_[i] = val;
        update_max_nonzero_index();
    }
}

void OddPolynomial::set_coef(const std::vector<double>& coefs)
{
    coefs_ = std::vector<double>(num_coefs_, 0.0);

    for(int i = 0; i < std::min(num_coefs_, (int)coefs.size()); i++){
        coefs_[i] = coefs[i];
    }

    update_max_nonzero_index();
}


double OddPolynomial::conic() const
{
    return conic_;
}

double OddPolynomial::coef(int i) const
{
    if(i < num_coefs_){
        return coefs_[i];
    }else{
        return 0.0;
    }
}

int OddPolynomial::coef_count() const
{
    return coefs_.size();
}

void OddPolynomial::update_max_nonzero_index()
{
    int i = 0;
    while(i < num_coefs_){
        if(fabs(coefs_[i]) < std::numeric_limits<double>::epsilon()){
            max_nonzero_index_ = i;
            break;
        }
        i++;
    }
}

double OddPolynomial::sag(double x, double y) const
{
    double r2 = x*x + y*y;
    double r = sqrt(r2);

    // conic contribution
    double z_conic = cv_*r2/( 1.0 + sqrt( 1.0 - cv_*cv_*r2*(conic_+1.0) ) );

    // polynomial contribution
    double z_pol = 0.0;
    double r_pow = r2*r;

    for(int i = 0; i < num_coefs_; i++) {
        z_pol += coefs_[i] * r_pow;
        r_pow *= r;
    }

    return (z_conic + z_pol);
}

double OddPolynomial::f(const Eigen::Vector3d &p) const
{
    return ( p(2) - this->sag(p(0), p(1)) );
}

Eigen::Vector3d OddPolynomial::df(const Eigen::Vector3d &p) const
{
    double r2 = p(0)*p(0) + p(1)*p(1);
    double r = sqrt(r2);
    double t = sqrt( 1.0 - cv_*cv_*r2*(conic_ + 1.0) ); // common sqrt

    // conic contribution
    double conic_contrib_1 = 2*cv_/(t + 1.0);
    double conic_contrib_2 = pow(cv_,3)*r2*(conic_+1.0)/( t*pow((t+1.0), 2) );

    // polynomial contribution
    double pol_contrib = 0.0;

    double r_pow = r;
    double num = 3.0;
    for(int i = 0; i < num_coefs_; i++){
        pol_contrib += num*coefs_[i]*r_pow;
        num += 1.0;
        r_pow *= r;
    }

    double e_tot = conic_contrib_1 + conic_contrib_2 + pol_contrib;

    Eigen::Vector3d df_ret({-e_tot*p(0), -e_tot*p(1), 1.0});
    return df_ret;
}

double OddPolynomial::deriv_1st(double h) const
{
    double t = sqrt( 1.0 - cv_*cv_*h*h*(conic_ + 1.0) ); // common sqrt

    // conic contribution
    double conic_contrib_1 = 2*cv_*h/(t + 1.0);
    double conic_contrib_2 = pow(cv_,3)*pow(h,3)*(conic_+1.0)/( t*pow((t+1.0), 2) );

    // polynomial contribution
    double pol_contrib = 0.0;
    double h_pol = h*h;
    double num = 3.0;
    for(int i = 0; i < num_coefs_; i++){
        pol_contrib += num*coefs_[i]*h_pol;
        num += 1.0;
        h_pol *= h;
    }

    double tot = conic_contrib_1 + conic_contrib_2 + pol_contrib;

    return tot;
}

double OddPolynomial::deriv_2nd(double h) const
{
    // conic contribution
    double k = conic_;
    double z_sqrt = sqrt(1.0 - cv_*cv_*h*h*(k+1.0)); //common sqrt

    double z1 = 2*cv_ / (1.0 + z_sqrt);

    double z2_denom = 5*pow(cv_,3)*pow(h,2)*(k+1.0);
    double z2_num = z_sqrt*pow(1.0 + z_sqrt, 2);
    double z2 = z2_denom/z2_num;

    double z3_denom = pow(cv_,5)*pow(h,4)*pow(k+1.0, 2);
    double z3_num = pow(z_sqrt, 3) * pow(z_sqrt + 1.0 , 2);
    double z3 = z3_denom/z3_num;

    double z4_denom = -2*pow(cv_,5)*pow(h,4)*pow(k+1.0, 2);
    double z4_num = ( cv_*cv_*h*h*(k+1.0) - 1.0 ) * pow( 1.0 + z_sqrt , 3);
    double z4 = z4_denom/z4_num;

    double z_conic = z1 + z2 + z3 + z4;

    // polynomial contribution
    double z_pol = 0.0;
    double h_pow = h;

    for(int i = 0; i < num_coefs_; i++) {
        double n = (double) ((i+2)*(i+3));
        z_pol += n*h_pow*coefs_[i];

        h_pow *= h;
    }

    return (z_conic + z_pol);

}
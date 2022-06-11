/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi All Rights Reserved.
** 
** This file is part of Geopter.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
** 
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi                                   
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com                          
**             Date: May 16th, 2021                                                                                          
********************************************************************************/

#include <iomanip>
#include "profile/odd_polynomial.h"
#include "sequential/trace_error.h"

using namespace geopter;

OddPolynomial::OddPolynomial() :
    conic_(0.0),
    max_nonzero_index_(0),
    num_coefs_(10)
{
    cv_ = 0.0;
    eps_ = 1.0e-5;
}

OddPolynomial::OddPolynomial(double cv, double conic, const std::vector<double>& coefs) :
    conic_(conic),
    max_nonzero_index_(0),
    num_coefs_(10)
{
    cv_ = cv;
    this->set_coef(coefs);
    eps_ = 1.0e-5;
}


OddPolynomial::~OddPolynomial()
{
    coefs_.clear();
}


bool OddPolynomial::intersect(Eigen::Vector3d& pt, double& distance, const Eigen::Vector3d& p0, const Eigen::Vector3d& dir)
{
    // Spencer's method

    Eigen::Vector3d p = p0;
    double s1 = -f(p)/dir.dot(df(p));
    double s2;
    double delta = fabs(s1);
    constexpr int max_iter = 30;
    int iter = 0;

    while(delta > eps_)
    {
        p = p0 + s1*dir;
        s2 = s1 - f(p)/dir.dot(df(p));
        delta = fabs(s2-s1);
        s1 = s2;
        iter++;

        if(iter > max_iter){
            pt = p;
            distance = s1;
            return false;
        }
    }

    pt = p;
    distance = s1;

    return true;
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


double OddPolynomial::coef(int i) const
{
    if(i < num_coefs_){
        return coefs_[i];
    }else{
        return 0.0;
    }
}


void OddPolynomial::update_max_nonzero_index()
{
    max_nonzero_index_ = num_coefs_-1;

    int i = num_coefs_-1;
    while(i > 0){
        if(fabs(coefs_[i]) > std::numeric_limits<double>::epsilon()){
            max_nonzero_index_ = i;
            break;
        }
        i--;
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

void OddPolynomial::print(std::ostringstream &oss)
{
    update_max_nonzero_index();

    constexpr int label_w = 6;
    //constexpr int val_w = 16;
    constexpr int prec  = 6;

    oss << std::setw(label_w) << std::left << "Type";
    oss << std::setw(label_w) << std::right << std::fixed << "Even Polynomial" << std::endl;

    oss << std::setw(label_w) << std::left << "R";
    oss << std::setw(label_w) << std::right << std::fixed << std::setprecision(prec) << (1.0/cv_) << std::endl;

    oss << std::setw(label_w) << std::left << "k";
    oss << std::setw(label_w) << std::right << std::fixed << std::setprecision(prec) << conic_ << std::endl;

    for(int i = 0; i <= max_nonzero_index_; i++){
        int coef_index = 3 + i;
        std::string coef_label = "A" + std::to_string(coef_index);
        oss << std::setw(label_w) << std::left << coef_label;
        oss << std::setw(label_w) << std::right << std::fixed << std::scientific << std::setprecision(prec) << coefs_[i] << std::endl;
    }

}

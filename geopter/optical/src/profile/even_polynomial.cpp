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

#include <iostream>
#include <iomanip>
#include "profile/even_polynomial.h"
#include "sequential/trace_error.h"

using namespace geopter;

EvenPolynomial::EvenPolynomial() :
    conic_(0.0),
    max_nonzero_index_(0),
    num_coefs_(10)
{
    cv_ = 0.0;
    coefs_ = std::vector<double>(num_coefs_, 0.0);
    eps_ = 1.0e-5;
}

EvenPolynomial::EvenPolynomial(double cv, double conic, const std::vector<double>& coefs) :
    conic_(conic),
    max_nonzero_index_(0),
    num_coefs_(10)
{
    cv_ = cv;
    coefs_ = std::vector<double>(num_coefs_, 0.0);
    this->set_coef(coefs);
    eps_ = 1.0e-5;
}


EvenPolynomial::~EvenPolynomial()
{
    coefs_.clear();
}


bool EvenPolynomial::intersect(Eigen::Vector3d& pt, double& distance, const Eigen::Vector3d& p0, const Eigen::Vector3d& dir)
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


double EvenPolynomial::sag(double x, double y) const
{
    double r2 = x*x + y*y;

    // sphere + conic contribution
    double z = 0.0;
    double inside_sqrt = 1.0 - (conic_+1.0)*cv_*cv_*r2;
    if(inside_sqrt < 0.0){
        std::cout << "TraceMissedSurface EvenPolynomial::sag()" << std::endl;
        return NAN;
    }
    else{
        //z = cv_*r2 / ( 1.0 + sqrt(1.0 - (conic_+1.0)*cv_*cv_*r2) );
        z = cv_*r2 / ( 1.0 + sqrt( inside_sqrt ) );
    }

    // polynomial contribution
    double z_asp = 0.0;
    double r_pow = r2;

    //Ar4 + Br6 + Cr8...
    for(int i = 0; i < num_coefs_; i++){
        r_pow *= r2;
        z_asp += coefs_[i]*r_pow;        
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
    double h_pow = h*h;
    for(int i = 0; i < num_coefs_; i++) {
        double n = (2*(i+1)+1) * (2*(i+1)+1 +1);
        z5 += n * coefs_[i] * h_pow;
        h_pow *= h*h;
    }

    return (z1 + z2 + z3 + z4 + z5);
}


void EvenPolynomial::print(std::ostringstream &oss)
{
    update_max_nonzero_index();

    constexpr int label_w = 6;
    //constexpr int val_w = 16;
    constexpr int prec  = 6;

    oss << std::setw(label_w) << std::left << "Type: ";
    oss << std::setw(label_w) << std::right << std::fixed << "Even Polynomial" << std::endl;

    oss << std::setw(label_w) << std::left << "R";
    oss << std::setw(label_w) << std::right << std::fixed << std::setprecision(prec) << (1.0/cv_) << std::endl;

    oss << std::setw(label_w) << std::left << "k";
    oss << std::setw(label_w) << std::right << std::fixed << std::setprecision(prec) << conic_ << std::endl;

    for(int i = 0; i <= max_nonzero_index_; i++){
        int coef_index = 4 + 2*i;
        std::string coef_label = "A" + std::to_string(coef_index);
        oss << std::setw(label_w) << std::left << coef_label;
        oss << std::setw(label_w) << std::right << std::fixed << std::scientific << std::setprecision(prec) << coefs_[i] << std::endl;
    }

}

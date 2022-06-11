/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
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

#ifndef EVENPOLYNOMIAL_H
#define EVENPOLYNOMIAL_H

#include <vector>
#include <string>
#include "Eigen/Core"

namespace geopter {

/** Even polynomial aspherical shape */
class EvenPolynomial
{
public:

    EvenPolynomial();
    EvenPolynomial(double cv, double conic = 0.0, const std::vector<double>& coefs = std::vector<double>(10, 0.0));
    ~EvenPolynomial();

    std::string name() const{
        return "ASP";
    }

    /** Returns the conic factor */
    double conic() const {
        return conic_;
    }

    /** Returns aspherical coefficient at the specified index */
    double coef(int i) const;

    /** Returns number of coefficients */
    int coef_count() const {
        return coefs_.size();
    }

    double sag(double x, double y) const;
    double f(const Eigen::Vector3d& p) const;
    Eigen::Vector3d df(const Eigen::Vector3d& p) const;
    double deriv_1st(double h) const;
    double deriv_2nd(double h) const;

    void set_conic(double cc) {  conic_ = cc;}
    void set_coef(int i, double val);
    void set_coef(const std::vector<double>& coefs);

    bool intersect(Eigen::Vector3d& pt, double& distance, const Eigen::Vector3d& p0, const Eigen::Vector3d& dir);

    void print(std::ostringstream& oss);

protected:
    void update_max_nonzero_index();

    double cv_;
    double eps_;
    double conic_;
    int max_nonzero_index_;
    std::vector<double> coefs_;
    int num_coefs_;
};

} //namespace

#endif // EVENPOLYNOMIAL_H

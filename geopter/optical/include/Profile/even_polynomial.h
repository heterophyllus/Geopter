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

//============================================================================
/// \file   even_polynomial.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


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

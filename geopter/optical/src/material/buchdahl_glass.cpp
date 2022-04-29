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

#define _USE_MATH_DEFINES
#include <math.h>

#include "material/buchdahl_glass.h"

#include "spec/spectral_line.h"


using namespace geopter;

BuchdahlGlass::BuchdahlGlass(double nd, double vd) : Material(),
    wv0_(SpectralLine::d/1000.0),
    rind0_(nd),
    vd_(vd)
{
    n_ = nd;
    compute_coefs();
    //name_ = this->glass_code();
}

BuchdahlGlass::~BuchdahlGlass()
{

}

std::string BuchdahlGlass::name() const
{
    return std::to_string(n_) + ":" + std::to_string(vd_);
}

double BuchdahlGlass::rindex(double wv_nm) const
{
    double om = omega(wv_nm/1000.0 - wv0_);
    return rind0_ + v1_*om + v2_*pow(om,2);
}

std::string BuchdahlGlass::glass_code() const
{
    std::string nd_part = std::to_string( (long int)round((n_-1.0)*10000.0) );
    std::string vd_part = std::to_string( (int)round(vd_*100.0) );
    return nd_part + "." + vd_part;
}

double BuchdahlGlass::abbe_d() const
{
    return vd_;
}

void BuchdahlGlass::compute_coefs()
{
    //compute v1, v2
    const double b = -0.064667;
    const double m = -1.604048;
    const double wF = SpectralLine::F / 1000.0;
    const double wC = SpectralLine::C / 1000.0;

    double omF = omega(wF - wv0_);
    double omC = omega(wC - wv0_);
    double delta_om  = omF - omC;
    double delta_om2 = omF*omF - omC*omC;

    double dFC = (n_-1.0)/vd_;
    v2_ = ( dFC - b*delta_om ) / ( m*delta_om - delta_om2 );
    v1_ = b + m*v2_;
}

double BuchdahlGlass::omega(double delta_lambda) const
{
    return delta_lambda/( 1 + 2.5*delta_lambda );
}

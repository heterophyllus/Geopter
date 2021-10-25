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
/// \file   buchdahl_glass.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#ifndef BUCHDAHLGLASS_H
#define BUCHDAHLGLASS_H

#include "material.h"

namespace geopter{


/** Buchdahl chromatic coordinate model
 *
 *  This class is mainly used to deal imaginary glass defined with nd and vd.
 */
class BuchdahlGlass : public Material
{
public:
    BuchdahlGlass(double nd, double vd);
    ~BuchdahlGlass();

    double rindex(double wv_nm) const override;

    std::string glass_code() const;

    double abbe_d() const override;

private:
    void compute_coefs();

    /** Calculate the Buchdahl chromatic coordinate */
    double omega(double delta_lambda) const;

    double wv0_;
    double rind0_;
    double vd_;

    // coefs
    double v1_;
    double v2_;
};

}

#endif // BUCHDAHLGLASS_H

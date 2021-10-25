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
/// \file   dispersion_formula.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#ifndef DISPERSION_FORMULA_H
#define DISPERSION_FORMULA_H

#define _USE_MATH_DEFINES
#include <math.h>

#include <vector>

namespace geopter {


/** Definition of glass dispersion formula to compute refractive index at specified wavelength. */
class DispersionFormula
{
public:
    static double Schott(double lambdamicron, const std::vector<double>& c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2) + c[2]*pow(lambdamicron,-2) + c[3]*pow(lambdamicron,-4) + c[4]*pow(lambdamicron,-6) + c[5]*pow(lambdamicron,-8) );
    }
    static double Sellmeier1(double lambdamicron, const std::vector<double>& c){
        return sqrt( 1 + c[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[1]) + c[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[3]) + c[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[5]) );
    }
    static double Sellmeier2(double lambdamicron, const std::vector<double>& c){
        return sqrt( 1 + c[0] + c[1]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[2]) + c[3]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[4]) );
    }
    static double Sellmeier3(double lambdamicron, const std::vector<double>& c){
        return sqrt( 1 + c[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[1]) + c[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[3]) + c[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[5]) + c[6]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[7]) );
    }
    static double Sellmeier4(double lambdamicron, const std::vector<double>& c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[2]) + c[3]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[4]) );
    }
    static double Sellmeier5(double lambdamicron, const std::vector<double>& c){
        return sqrt( 1 + c[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[1]) + c[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[3]) + c[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[5]) + c[6]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[7]) + c[8]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[9]) );
    }
    static double Herzberger(double lambdamicron, const std::vector<double>& c){
        double L = 1/(pow(lambdamicron,2)-0.028);
        return ( c[0] + c[1]*L + c[2]*pow(L,2) + c[3]*pow(lambdamicron,2) + c[4]*pow(lambdamicron,4) + c[5]*pow(lambdamicron,6) );
    }
    static double HandbookOfOptics1(double lambdamicron, const std::vector<double>& c){
        return sqrt( c[0] + c[1]/(pow(lambdamicron,2)-c[2]) - c[3]*pow(lambdamicron,2) );
    }
    static double HandbookOfOptics2(double lambdamicron, const std::vector<double>& c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[2]) - c[3]*pow(lambdamicron,2) );
    }
    static double Extended1(double lambdamicron, const std::vector<double>& c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2) + c[2]*pow(lambdamicron,-2) + c[3]*pow(lambdamicron,-4) + c[4]*pow(lambdamicron,-6) + c[5]*pow(lambdamicron,-8) + c[6]*pow(lambdamicron,-10) + c[7]*pow(lambdamicron,-12) );
    }
    static double Extended2(double lambdamicron, const std::vector<double>& c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2) + c[2]*pow(lambdamicron,-2) + c[3]*pow(lambdamicron,-4) + c[4]*pow(lambdamicron,-6) + c[5]*pow(lambdamicron,-8) + c[6]*pow(lambdamicron,4) + c[7]*pow(lambdamicron,6) );
    }
    static double Conrady(double lambdamicron, const std::vector<double>& c){
        return ( c[0] + c[1]/lambdamicron + c[2]/pow(lambdamicron,3.5) );
    }

    static double Unknown(double lambdamicron, const std::vector<double>& c){
        return 1.0;
    }

    /*
    static double Laurent(double lambdamicron, std::vector<double> c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2) + c[2]*pow(lambdamicron,-2) + c[3]*pow(lambdamicron,-4) + c[4]*pow(lambdamicron,-6) + c[5]*pow(lambdamicron,-8) + c[6]*pow(lambdamicron,-10) + c[7]*pow(lambdamicron,-12) + c[8]*pow(lambdamicron,-14) + c[9]*pow(lambdamicron,-16) + c[10]*pow(lambdamicron,-18) + c[11]*pow(lambdamicron,-20) );
    }
    static double GlassManufacturerLaurent(double lambdamicron, std::vector<double> c){
        return sqrt( c[0] + c[1]*pow(lambdamicron,2) + c[2]*pow(lambdamicron,-2) + c[3]*pow(lambdamicron,-4) + c[4]*pow(lambdamicron,-6) + c[5]*pow(lambdamicron,-8) + c[6]*pow(lambdamicron,4) );
    }
    static double GlassManufacturerSellmeier(double lambdamicron, std::vector<double> c){
        return sqrt( 1 + c[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[1]) + c[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[3]) + c[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[5]) + c[6]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[7]) + c[8]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[9]) + c[10]*pow(lambdamicron,2)/(pow(lambdamicron,2)-c[11]) );
    }
    static double StandardSellmeier(double lambdamicron, std::vector<double> c){
        return sqrt( 1 + c[0]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[1],2)) + c[2]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[3],2)) + c[4]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[5],2)) + c[6]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[7],2)) + c[8]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[9],2)) + c[10]*pow(lambdamicron,2)/(pow(lambdamicron,2)-pow(c[11],2)) );
    }
    static double Cauchy(double lambdamicron, std::vector<double> c){
        return c[0] + c[1]*pow(lambdamicron,-2) + c[2]*pow(lambdamicron,-4);
    }
    static double Hartman(double lambdamicron, std::vector<double> c){
        return c[0] + c[1]/pow((c[2]-lambdamicron), 1.2);
    }
    */

    static double Nikon_Hikari(double lambdamicron, const std::vector<double>& c){
        // https://www.hikari-g.co.jp/products/nature/properties_optical_glass/
        return sqrt( c[0] + c[1]*pow(lambdamicron,2) + c[2]*pow(lambdamicron,4) + c[3]*pow(lambdamicron,-2) + c[4]*pow(lambdamicron,-6) + c[5]*pow(lambdamicron,-8) + c[6]*pow(lambdamicron,-8) + c[7]*pow(lambdamicron,-10) + c[8]*pow(lambdamicron, -12) );
    }

};

} //namespace geopter


#endif // DISPERSION_FORMULA_H

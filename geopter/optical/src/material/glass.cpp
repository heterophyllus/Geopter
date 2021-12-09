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

#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "material/glass.h"
#include "material/glass_catalog.h"
#include "material/dispersion_formula.h"
#include "material/air.h"
#include "environment/environment.h"
#include "spec/spectral_line.h"
#include "common/string_tool.h"

using namespace geopter;

Glass::Glass() : Material(),
    formula_func_ptr_(nullptr)
{
    constexpr int knum_coefs = 12;
    coefs_ = std::vector<double>(knum_coefs, 0.0);
}

Glass::~Glass()
{
    formula_func_ptr_ = nullptr;
    coefs_.clear();
}


double Glass::rindex(double wv_nm) const
{
    if(formula_func_ptr_){
        return refractive_index_rel(wv_nm/1000.0);
    }else{
        return 1.0;
    }
}


std::string Glass::name() const
{
    return product_name_ + "_" + supplier_name_;
}

void Glass::set_dispersion_formula(int i)
{
    switch (i) {
    case 1:
        formula_func_ptr_ = &(DispersionFormula::Schott);
        break;
    case 2:
        formula_func_ptr_ = &(DispersionFormula::Sellmeier1);
        break;
    case 3:
        formula_func_ptr_ = &(DispersionFormula::Herzberger);
        break;
    case 4:
        formula_func_ptr_ = &(DispersionFormula::Sellmeier2);
        break;
    case 5:
        formula_func_ptr_ = &(DispersionFormula::Conrady);
        break;
    case 6:
        formula_func_ptr_ = &(DispersionFormula::Sellmeier3);
        break;
    case 7:
        formula_func_ptr_ = &(DispersionFormula::HandbookOfOptics1);
        break;
    case 8:
        formula_func_ptr_ = &(DispersionFormula::HandbookOfOptics2);
        break;
    case 9:
        formula_func_ptr_ = &(DispersionFormula::Sellmeier4);
        break;
    case 10:
        formula_func_ptr_ = &(DispersionFormula::Extended1);
        break;
    case 11:
        formula_func_ptr_ = &(DispersionFormula::Sellmeier5);
        break;
    case 12:
        formula_func_ptr_ = &(DispersionFormula::Extended2);
        break;
    case 13: // Unknown
        if(StringTool::contains(supplier_name_, "HIKARI")){
            formula_func_ptr_ = &(DispersionFormula::Nikon_Hikari);
        }else{
            formula_func_ptr_ = &(DispersionFormula::Unknown);
        }
        break;
    default:
        formula_func_ptr_ = nullptr;
    }

}

void Glass::set_dispersion_coefs(int i, double val)
{
    if(i < (int)coefs_.size()){
        coefs_[i] = val;
    }else{
        return;
    }
}


std::string Glass::product_name() const
{
    return product_name_;
}

void Glass::set_product_name(std::string name)
{
    product_name_ = name;
    std::transform(product_name_.begin(), product_name_.end(), product_name_.begin(), toupper);
}

void Glass::set_supplier(std::string sup)
{
    supplier_name_ = sup;
    std::transform(supplier_name_.begin(), supplier_name_.end(), supplier_name_.begin(), toupper);
}

std::string Glass::supplier() const
{
    return supplier_name_;
}

double Glass::abbe_d() const
{
    double nd = rindex(SpectralLine::d);
    double nF = rindex(SpectralLine::F);
    double nC = rindex(SpectralLine::C);

    return (nd - 1.0)/(nF - nC);
}

double Glass::refractive_index_rel_Tref(double wvl_micron) const
{
    if(formula_func_ptr_){
        return formula_func_ptr_(wvl_micron, coefs_);
    }else{
        return 1.0;
    }
}


double Glass::refractive_index_abs_Tref(double wvl_micron) const
{
    constexpr double P = 101325.0;
    double n_air_T0 = Air::refractive_index_abs(wvl_micron, Tref_, P);
    double n_rel_T0 = refractive_index_rel_Tref(wvl_micron);
    double n_abs_T0 = n_rel_T0*n_air_T0;

    return n_abs_T0;
}

double Glass::refractive_index_abs(double wvl_micron) const
{
    double T = Environment::temperature();
    double dn_dt = dn_dt_abs(wvl_micron, T);
    double dn = (T-Tref_)*dn_dt;
    double n_abs_T0 = refractive_index_abs_Tref(wvl_micron);
    double n_abs = n_abs_T0 + dn;

    return n_abs;
}

double Glass::refractive_index_rel(double wvl_micron) const
{
    double T = Environment::temperature();
    double n_abs = refractive_index_abs(wvl_micron);
    double n_air = Air::refractive_index_abs(wvl_micron, T);
    double n_rel = n_abs/n_air;

    return n_rel;
}

void Glass::set_thermal_data(double D0, double D1, double D2, double E0, double E1, double Ltk, double Tref)
{
    D0_ = D0;
    D1_ = D1;
    D2_ = D2;
    E0_ = E0;
    E1_ = E1;
    Ltk_  = Ltk;
    Tref_ = Tref;
}

double Glass::dn_dt_abs(double wvl_micron, double t) const
{
    double dT = t - Tref_;
    double Stk = (Ltk_ > 0.0) - (Ltk_ < 0.0);
    double n  = refractive_index_abs_Tref(wvl_micron);

    return (n*n-1)/(2*n) * ( D0_ + 2*D1_*dT + 3*D2_*dT*dT + (E0_ + 2*E1_*dT)/(wvl_micron*wvl_micron - Stk*Ltk_*Ltk_) );
}

void Glass::print()
{
    std::ostringstream oss;
    print(oss);
    std::cout << oss.str() << std::endl;
}

void Glass::print(std::ostringstream &oss)
{
    //const int idx_w = 4;
    constexpr int val_w = 10;
    constexpr int prec  = 4;

    oss << "Glass: " << name_ << " (" << supplier_name_ << ")" << std::endl;
    oss << "nd: " << std::setw(val_w) << std::scientific << std::setprecision(prec) << n_ << std::endl;
    oss << "vd: " << std::setw(val_w) << std::scientific << std::setprecision(prec) << abbe_d() << std::endl;

    oss << "CD: ";
    for(int i = 0; i < (int)coefs_.size(); i++)
    {
        oss << std::scientific << std::setprecision(prec) << coefs_[i];
        oss << " ";
    }
    oss << std::endl;

}

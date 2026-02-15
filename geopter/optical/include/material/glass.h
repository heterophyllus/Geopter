/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
** 
** This file is part of Geopter.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
** 
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
** 
** You should have received a copy of the GNU General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi                                   
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com                          
**             Date: May 16th, 2021                                                                                          
********************************************************************************/


#ifndef GLASS_H
#define GLASS_H

#include <vector>
#include <string>
#include <algorithm>

#include "material.h"


namespace geopter {


/** Glass that is actually available */
class Glass : public Material
{
public:
    Glass();
    ~Glass();

    double RefractiveIndex(double wv_nm) const override;

    std::string Name() const override { return product_name_ + "_" + supplier_name_;}
    void SetName(const std::string& /*name*/) override { }

    void SetDispersionFormula(int i);
    void SetDispersionCoefs(int i, double val);

    double RefractiveIndexRel(double wvl_micron) const;
    double RefractiveIndexAbs(double wvl_micron) const;

    void SetSupplier(std::string sup){ supplier_name_ = sup; std::transform(supplier_name_.begin(), supplier_name_.end(), supplier_name_.begin(), toupper); }
    std::string Supplier() const { return supplier_name_;}

    void SetProductName(std::string name){ product_name_ = name; std::transform(product_name_.begin(), product_name_.end(), product_name_.begin(), toupper); }
    const std::string& ProductName() const { return product_name_; }

    double Abbe_d() const override;

    void SetThermalData(double D0, double D1, double D2, double E0, double E1, double Ltk, double Tref);
    double DnDtAbs(double wvl_micron, double t) const;
    double Delta_n_Abs(double wvl_micron, double t) const;

    void Print();
    void Print(std::ostringstream& oss);

private:
    double RelativeWavelength(double lambdainput, double T, double P = 101325.0) const;
    double RefractiveIndexAbs_Tref(double wvl_micron) const;
    double RefractiveIndexRel_Tref(double wvl_micron) const;

    /** dispersion formula */
    double (*formula_func_ptr_)(double, const std::vector<double>&);

    /** dispersion coefficients */
    std::vector<double> coefs_;

    /** glass maker name (ex. SCHOTT, OHARA, etc) */
    std::string supplier_name_;

    /** product name (ex. N-BK7 etc) */
    std::string product_name_;

    /** Thermal coefficient data */
    double D0_;
    double D1_;
    double D2_;
    double E0_;
    double E1_;
    double Ltk_;

    /** reference temperature of the glass */
    double Tref_;

    double Pref_;
};

} //namespace geopter

#endif // GLASS_H

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

#include "material.h"


namespace geopter {


/** Glass that is actually available */
class Glass : public Material
{
public:
    Glass();
    ~Glass();

    double rindex(double wv_nm) const override;

    std::string name() const override;

    void set_dispersion_formula(int i);
    void set_dispersion_coefs(int i, double val);

    double refractive_index_rel(double wvl_micron) const;
    double refractive_index_abs(double wvl_micron) const;

    void set_supplier(std::string sup);
    std::string supplier() const;

    void set_product_name(std::string name);
    std::string product_name() const;

    double abbe_d() const override;

    void set_thermal_data(double D0, double D1, double D2, double E0, double E1, double Ltk, double Tref);
    double dn_dt_abs(double wvl_micron, double t) const;
    double delta_n_abs(double wvl_micron, double t) const;

    void print();
    void print(std::ostringstream& oss);

private:
    double relative_wavelength(double lambdainput, double T, double P = 101325.0) const;
    double refractive_index_abs_Tref(double wvl_micron) const;
    double refractive_index_rel_Tref(double wvl_micron) const;

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

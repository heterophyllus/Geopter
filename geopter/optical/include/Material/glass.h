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

    void print();
    void print(std::ostringstream& oss);

private:

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


};

} //namespace geopter

#endif // GLASS_H

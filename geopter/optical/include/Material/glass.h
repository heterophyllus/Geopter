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

    void compute_refractive_index();

    void set_supplier(std::string sup);
    std::string supplier() const;

    void set_product_name(std::string name);
    std::string product_name() const;

    double abbe_d() const;

    void print();
    void print(std::ostringstream& oss);

private:
    /** dispersion formula */
    double (*formula_func_ptr_)(double, const std::vector<double>&);

    /** dispersion coefficients */
    std::vector<double> coefs_;

    /** glass maker name (ex. SCHOTT, OHARA, etc) */
    std::string supplier_name_;

    /** product name (ex. N-BK7 etc) */
    std::string product_name_;
};

} //namespace geopter

#endif // GLASS_H

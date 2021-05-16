#ifndef GLASS_H
#define GLASS_H

#include <vector>

#include "medium.h"


namespace geopter {


/** Glass that is actually available */
class Glass : public Medium
{
public:
    Glass();
    ~Glass();

    std::string glass_code();

    std::string display_name() const override;

    double rindex(double wv_nm) const override;

    double abbe_d() const;

    void set_dispersion_formula(int i);
    void set_dispersion_coefs(int i, double val);

    void compute_refractive_index();

    bool search_from_catalog(std::string catname);

    void set_supplier(std::string sup);
    std::string supplier() const;

    void print();
    void print(std::ostringstream& oss);

private:
    double v_;

    double (*formula_func_ptr_)(double, const std::vector<double>&);

    std::vector<double> coefs_;

    /** glass maker name */
    std::string supplier_;

};

} //namespace geopter

#endif // GLASS_H

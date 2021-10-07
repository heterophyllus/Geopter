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

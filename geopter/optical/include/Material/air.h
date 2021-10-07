#ifndef AIR_H
#define AIR_H

#include "material.h"

namespace geopter {


class Air : public Material
{
public:
    Air();
    ~Air();

    double rindex(double wv_nm) const override;

    static double refractive_index_abs(double wvl_micron, double T, double P= 101325.0);

private:
    static double refractive_index_reference(double wvl_micron);
};



} // namespace

#endif // AIR_H

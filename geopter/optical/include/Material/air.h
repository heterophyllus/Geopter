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
};



} // namespace

#endif // AIR_H

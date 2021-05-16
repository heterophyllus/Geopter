#ifndef AIR_H
#define AIR_H

#include "medium.h"

namespace geopter {


class Air : public Medium
{
public:
    Air();
    ~Air();

    double rindex(double wv_nm) const override;
};



} // namespace

#endif // AIR_H

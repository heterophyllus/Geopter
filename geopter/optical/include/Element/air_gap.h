#ifndef AIR_GAP_H
#define AIR_GAP_H

#include "Element/element.h"
#include "Assembly/gap.h"

namespace geopter {


class AirGap : public Element
{
public:
    AirGap(Gap* g);
    ~AirGap();
    
private:
    Gap* gap_;
};

} // namespace

#endif

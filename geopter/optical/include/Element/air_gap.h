#ifndef AIR_GAP_H
#define AIR_GAP_H

#include "element.h"

namespace geopter {

class Gap;

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
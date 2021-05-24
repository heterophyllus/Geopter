#ifndef PATH_H
#define PATH_H

#include "transform.h"

namespace geopter {

class Surface;
class Gap;

struct PathComponent{
    PathComponent(){
        srf = nullptr;
        gap = nullptr;
        rndx = 1.0;
        tfrm.transfer = Eigen::Vector3d::Zero(3);
        tfrm.rotation = Eigen::Matrix3d::Identity(3,3);
        z_dir = 1.0;
    }
    ~PathComponent(){
        srf = nullptr;
        gap = nullptr;
    }

    Surface* srf;
    Gap* gap;
    double rndx;
    Tfrm tfrm;
    double z_dir;
};

using Path = std::vector<PathComponent>;

}

#endif // PATH_H

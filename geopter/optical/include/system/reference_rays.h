
#include <vector>
#include "sequential/ray.h"

namespace geopter {

/** Container for reference rays */
class ReferenceRays
{
public:
    ReferenceRays();
    ~ReferenceRays();

    void allocate(int num_fld, int num_wvl);
    void clear();

    RayPtr chief_ray(int f, int w);

private:
    std::vector< std::vector<RayPtr> > chief_rays_;
    std::vector< std::vector<RayPtr> > meridional_upper_rays_;
    std::vector< std::vector<RayPtr> > meridional_lower_rays_;
    std::vector< std::vector<RayPtr> > sagittal_upper_rays_;
    std::vector< std::vector<RayPtr> > sagittal_lower_rays_;

    int num_srfs_;
};

} // namespace geopter


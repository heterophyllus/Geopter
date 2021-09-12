#ifndef SEQUENTIALMODEL_H
#define SEQUENTIALMODEL_H

#include <vector>
#include <memory>
#include <sstream>

#include "Sequential/ray.h"

namespace geopter {

class OpticalSystem;

/** Class for sequantial tracing and analysis */
class SequentialModel
{
public:
    SequentialModel(OpticalSystem* sys);
    ~SequentialModel();

    /**
     * @brief Returns reference ray object
     * 
     * @param ri ray index
     * @param fi field index
     * @param wi wavelength index
     * @return Ray ray object
     */
    Ray reference_ray(int ri, int fi, int wi) const;

    
    /** Compute vignetting factors using current aperture setting */
    void compute_vignetting();


    /** List up model properties */
    void print(std::ostringstream& oss);

    void update_model();


private:
    void update_reference_rays();

    /** reference rays */
    std::vector<Ray> ref_rays1_; // chief ray
    std::vector<Ray> ref_rays2_; // upper meridional marginal
    std::vector<Ray> ref_rays3_; // lower meridional marginal
    std::vector<Ray> ref_rays4_; // upper sagittal marginal
    std::vector<Ray> ref_rays5_; // lower sagittal marginal

    /** focus shifts */
    std::vector<double> x_focus_shifts_;
    std::vector<double> y_focus_shifts_;
    
    OpticalSystem* opt_sys_;
    int num_fld_;
    int num_wvl_;
    int ref_wvl_idx;

    inline int to_ray_index(int fi, int wi) const;
};


int SequentialModel::to_ray_index(int fi, int wi) const
{
    return num_fld_*(fi) + wi;
}

} //namespace geopter


#endif // SEQUENTIALMODEL_H

#ifndef ABERRATION_H
#define ABERRATION_H

#include "System/optical_system.h"
#include "Renderer/renderer.h"

namespace geopter{

/** Analysis regarding ray aberration */
class Aberration
{
public:
    Aberration(OpticalSystem* opt_sys, Renderer* renderer);
    ~Aberration();

    /** @brief Plot transverse ray fan 
     *  @param scale plot scale
     *  @param nrd number of rays per pupil diameter
    */
    void plot_transverse_aberration(double scale, double nrd);

    void plot_longitudinal_spherical_aberration(double scale);

    void plot_astigmatism(double scale);

    void plot_chromatic_focus_shift(double lower_wvl, double higher_wvl);

    void plot_spot_diagram(int nrd, double scale, double dot_size);

private:
    OpticalSystem* opt_sys_;
    Renderer* renderer_;
    int num_fld_;
    int num_wvl_;
    double ref_wvl_val_;
    int ref_wvl_idx_;
};

}


#endif

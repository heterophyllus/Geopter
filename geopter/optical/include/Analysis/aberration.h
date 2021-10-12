#ifndef ABERRATION_H
#define ABERRATION_H

#include "System/optical_system.h"
#include "Renderer/renderer.h"
#include "Data/plot_data.h"

namespace geopter{

/** Analysis regarding ray aberration */
class Aberration
{
public:
    Aberration(OpticalSystem* opt_sys, Renderer* renderer);
    ~Aberration();

    /** @brief Plot transverse ray fan for 1 field
     *  @param nrd number of rays per pupil diameter
     *  @param fld
     *  @param pupil_dir X orY
     *  @param abr_dir DX or DY
    */
    std::shared_ptr<PlotData> plot_transverse_aberration(double nrd, const Field* fld, int pupil_dir= 1, int abr_dir= 1);

    std::shared_ptr<PlotData> plot_longitudinal_spherical_aberration(int num_rays= 10);

    std::shared_ptr<PlotData> plot_astigmatism(int ray_aiming_type, int num_rays= 10);

    std::shared_ptr<PlotData> plot_chromatic_focus_shift(double lower_wvl, double higher_wvl);

    std::shared_ptr<PlotData> plot_spot_diagram(const Field* fld, int pattern, int nrd, double dot_size);

private:
    std::vector<Eigen::Vector2d> create_grid_circle(int nrd);
    std::vector<Eigen::Vector2d> create_hexapolar(int nrd);

    enum SpotRayPattern{
        Grid,
        Hexapolar
    };

    OpticalSystem* opt_sys_;
    Renderer* renderer_;
    int num_fld_;
    int num_wvl_;
    double ref_wvl_val_;
    int ref_wvl_idx_;
};

}


#endif

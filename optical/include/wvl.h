#ifndef WVL_H
#define WVL_H

#include "spectral_line.h"
#include "rgb.h"

namespace geopter{

/** Component of WvlSpec. */
class Wvl
{
public:
    Wvl(double wl=SpectralLine::d, double wt=1.0, Rgb render_color=rgb_black);
    ~Wvl();

    /**
     * @brief Set wavelength value
     * @param val_nm value in nm
     */
    void set_value(double val_nm);

    /** Return wavelength value in nm */
    double value() const;

    void set_weight(double wt);
    double weight() const;

    void set_render_color(Rgb color);
    Rgb render_color() const;

private:
    /** wavelength in nm */
    double value_;

    /** wavelength weight used for MTF calculation, optimization, etc */
    double weight_;

    /** Render color used to draw ray line, plot ray fans, etc */
    Rgb render_color_;
};

} //namespace geopter

#endif // WVL_H

#ifndef WVL_H
#define WVL_H

#include "spectral_line.h"
#include "Renderer/rgb.h"

namespace geopter{

/** Wavelength component class */
class Wvl
{
public:
    Wvl(double wl=SpectralLine::d, double wt=1.0, Rgb render_color=rgb_black);
    ~Wvl();

    /**
     * @brief Set wavelength value
     * @param val_nm value in nm
     */
    inline void set_value(double val_nm);

    /** Returns wavelength value in nm */
    inline double value() const;

    inline void set_weight(double wt);

    /** Returns the weight */
    inline double weight() const;

    inline void set_render_color(const Rgb& color);

    /** Returns render color */
    inline Rgb render_color() const;

private:
    /** wavelength in nm */
    double value_;

    /** wavelength weight used for MTF calculation, optimization, etc */
    double weight_;

    /** Render color used to draw ray line, plot ray fans, etc */
    Rgb render_color_;
};


void Wvl::set_value(double val_nm)
{
    value_ = val_nm;
}

double Wvl::value() const
{
    return value_;
}

void Wvl::set_weight(double wt)
{
    weight_ = wt;
}

double Wvl::weight() const
{
    return weight_;
}

void Wvl::set_render_color(const Rgb& color)
{
    render_color_ = color;
}

Rgb Wvl::render_color() const
{
    return render_color_;
}


} //namespace geopter

#endif // WVL_H

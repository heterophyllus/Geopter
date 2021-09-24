#ifndef WVLSPEC_H
#define WVLSPEC_H


#include <vector>
#include <memory>

#include "wvl.h"

namespace geopter {

class WvlSpec
{
public:
    WvlSpec();
    ~WvlSpec();

    /** Returns Wvl component at the specified index */
    Wvl* wvl(int i) const;

    /** Returns number of wvl */
    int wvl_count() const;

    /** Get wavelength value of current reference index */
    double reference_wvl() const;

    /** Set reference index to the given index */
    void set_reference_index(int i);

    /** Returns current reference wavelength index */
    int reference_index() const;

    /** Returns minimum wavelength value in current spectral region */
    double lower_wavelength() const;

    /** Returns maximum wavelength value in current spectral region */
    double higher_wavelength() const;


    /** Aliase to wavelength value at the specified index */
    double wavelength(int i) const;

    /** Add a new wavelength */
    void add(double wl, double wt= 1.0, Rgb render_color= rgb_black);

    /** Remove Wvl component at the specified index */
    void remove(int i);

    void clear();

    void print();
    void print(std::ostringstream& oss);

private:
    int reference_index_;

    std::vector< std::unique_ptr<Wvl> > wvls_;
};

} //namespace geopter

#endif // WVLSPEC_H

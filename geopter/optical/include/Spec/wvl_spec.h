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
    inline Wvl* wvl(int i) const;

    /** Returns number of wvl */
    inline int wvl_count() const;

    /** Get wavelength value of current reference index */
    inline double reference_wvl() const;

    /** Returns current reference wavelength index */
    inline int reference_index() const;

    /** Returns minimum wavelength value in current spectral region */
    double lower_wavelength() const;

    /** Returns maximum wavelength value in current spectral region */
    double higher_wavelength() const;

    /** Set reference index to the given index */
    inline void set_reference_index(int i);

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



Wvl* WvlSpec::wvl(int i) const
{
    return wvls_[i].get();
}

int WvlSpec::wvl_count() const
{
    return (int)wvls_.size();
}

int WvlSpec::reference_index() const
{
    return reference_index_;
}

double WvlSpec::reference_wvl() const
{
    return wvls_[reference_index_]->value();
}


void WvlSpec::set_reference_index(int i)
{
    reference_index_ = i;
}

} //namespace geopter

#endif // WVLSPEC_H

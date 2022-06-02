/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi All Rights Reserved.
** 
** This file is part of Geopter.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
** 
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
********************************************************************************
**           Author: Hiiragi                                   
**          Website: https://github.com/heterophyllus/Geopter
**          Contact: heterophyllus.work@gmail.com                          
**             Date: May 16th, 2021                                                                                          
********************************************************************************/

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

    /** Set reference index to the given index */
    inline void set_reference_index(int i);

    /** Returns minimum wavelength value in current spectral region */
    inline double lower_wavelength() const;

    /** Returns maximum wavelength value in current spectral region */
    inline double higher_wavelength() const;

    /** Returns wavelength value list */
    std::vector<double> get_wavelength_list() const;

    /** Returns wavelength weight list */
    std::vector<double> get_weight_list() const;

    /** Returns max weight */
    inline double max_weight() const;

    /** Add a new wavelength */
    void add(double wl, double wt= 1.0, Rgb render_color= rgb_black);

    /** Remove Wvl component at the specified index */
    void remove(int i);

    void clear();

    void print();
    void print(std::ostringstream& oss);

private:
    void update();

    std::vector< std::unique_ptr<Wvl> > wvls_;
    int reference_index_;
    double higher_;
    double lower_;
    double max_weight_;
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

double WvlSpec::lower_wavelength() const
{
    return lower_;
}

double WvlSpec::higher_wavelength() const
{
    return higher_;
}

double WvlSpec::max_weight() const
{
    return max_weight_;
}


} //namespace geopter

#endif // WVLSPEC_H

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

#ifndef WAVELENGTHSPEC_H
#define WAVELENGTHSPEC_H


#include <vector>
#include <memory>

#include "wavelength.h"

namespace geopter {

class WavelengthSpec
{
public:
    WavelengthSpec();
    ~WavelengthSpec();

    int NumberOfWavelengths() { return num_wvls_; }

    /** Returns Wvl component at the specified index */
    Wavelength* GetWavelength(int i) const { return wvls_[i].get();}

    /** Get wavelength value of current reference index */
    double ReferenceWavelength() const { return wvls_[reference_index_]->Value();}

    /** Returns current reference wavelength index */
    int ReferenceIndex() const { return reference_index_;}

    /** Set reference index to the given index */
    void SetReferenceIndex(int i) { reference_index_ = i;}

    /** Returns minimum wavelength value in current spectral region */
    double LowerWavelength() const { return lower_;}

    /** Returns maximum wavelength value in current spectral region */
    double HigherWavelength() const { return higher_;}

    /** Returns wavelength value list */
    std::vector<double> GetWavelengthList() const;

    /** Returns wavelength weight list */
    std::vector<double> GetWeightList() const;

    /** Returns max weight */
    double MaxWeight() const {return max_weight_;}

    /** Add a new wavelength */
    void AddWavelength(double wl, double wt= 1.0, Rgb render_color= rgb_black);

    /** Remove wavelength at the specified index */
    void RemoveWavelength(int i);

    void clear();

    void print();
    void print(std::ostringstream& oss);

private:
    void update();

    std::vector< std::unique_ptr<Wavelength> > wvls_;
    int reference_index_;
    double higher_;
    double lower_;
    double max_weight_;

    int num_wvls_;
};



} //namespace geopter

#endif // WVLSPEC_H

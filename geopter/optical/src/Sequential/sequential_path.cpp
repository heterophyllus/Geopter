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

//============================================================================
/// \file   sequential_path.cpp
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#include <cassert>

#include "sequential/sequential_path.h"
#include "common/geopter_error.h"
#include "spec/spectral_line.h"

using namespace geopter;

SequentialPath::SequentialPath() :
    wvl_(SpectralLine::d),
    array_size_(0)
{

}

SequentialPath::~SequentialPath()
{
    seq_path_comps_.clear();
}

int SequentialPath::size() const
{
    assert(array_size_ == (int)seq_path_comps_.size());
    return array_size_;
}

void SequentialPath::clear()
{
    seq_path_comps_.clear();
    array_size_ = 0;
}

void SequentialPath::append(SequentialPathComponent seq_path_comp)
{
    seq_path_comps_.push_back(seq_path_comp);
    array_size_ += 1;
}

void SequentialPath::append(Surface *s, double thi, double n)
{
    SequentialPathComponent seq_path_comp(s, thi, n);
    seq_path_comps_.push_back(seq_path_comp);
    array_size_ += 1;
}

SequentialPathComponent SequentialPath::at(int i) const
{
    assert(array_size_ == (int)seq_path_comps_.size());

    if(i < array_size_) {
        return seq_path_comps_[i];
    } else {
        throw OutOfRangeError();
    }
}

double SequentialPath::wvl() const
{
    return wvl_;
}

void SequentialPath::set_wvl(double wvl)
{
    wvl_ = wvl;
}


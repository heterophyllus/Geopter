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
**             Date: November 16th, 2021
********************************************************************************/

#ifndef GEOPTER_HEXAPOLAR_ARRAY_H
#define GEOPTER_HEXAPOLAR_ARRAY_H

#define _USE_MATH_DEFINES
#include <cmath>

#include <vector>

namespace geopter{

template<class T>
class HexapolarArray
{
public:
    HexapolarArray(int nrd);
    ~HexapolarArray();

    auto & operator [](int n) { return data_[n]; }

    T at(int ring_index, int azimuth_index);
    int total_number_of_points() const;
    static int points_in_ring(int ri);
    static int ring_azimuth_to_index(int ring_index, int azimuth_index);

    private:
    int nrd_;
    int rings_;

    std::vector<T> data_;
};

template<class T>
HexapolarArray<T>::HexapolarArray(int nrd)
{
    nrd_ = nrd;

    if(nrd % 2 == 0) {
        rings_ = nrd/2;
    }else{
        rings_ = (nrd-1)/2;
    }

    int total_num_pts = 0;
    for(int ri = 0; ri <= rings_; ri++){
        total_num_pts += points_in_ring(ri);
    }

    data_.resize(total_num_pts);
}

template<class T>
HexapolarArray<T>::~HexapolarArray()
{
    data_.clear();
}


template<class T>
T HexapolarArray<T>::at(int ring_index, int azimuth_index)
{
    return data_[ring_azimuth_to_index(ring_index, azimuth_index)];
}

template<class T>
int HexapolarArray<T>::total_number_of_points() const
{
    int total_num_pts = 0;
    for(int ri = 0; ri <= rings_; ri++){
        total_num_pts += points_in_ring(ri);
    }

    return total_num_pts;
}

template<class T>
int HexapolarArray<T>::points_in_ring(int ri)
{
    if(ri == 0){
        return 1;
    }else{
        return ri*6;
    }
}

template<class T>
int HexapolarArray<T>::ring_azimuth_to_index(int ring_index, int azimuth_index)
{
    if(ring_index == 0) {
        return 0;
    }

    int ring_start_index = 0;
    for(int i = 1; i < ring_index; i++){
        ring_start_index += points_in_ring(i);
    }
    ring_start_index += 1;

    return ring_start_index + azimuth_index;
}

} //namespace geopter


#endif //GEOPTER_HEXAPOLAR_ARRAY_H


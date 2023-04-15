/*******************************************************************************
** Geopter
** Copyright (C) 2021 Hiiragi
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


#ifndef PUPILSPEC_H
#define PUPILSPEC_H

#include <cassert>
#include <sstream>

namespace geopter {

enum PupilType
{
    EPD,
    FNO,
    NAO,
    NA
};


class PupilSpec
{

public:
    PupilSpec();
    PupilSpec(int pupil_type, double value);
    ~PupilSpec();

    int PupilType() const { return pupil_type_; }

    double Value() const { return value_; }

    void SetPupilType(int i) { pupil_type_ = i; }

    void SetValue(double val) { value_ = val; }

    void Print(std::ostringstream& oss);

private:
    int pupil_type_;
    double value_;
};



} //namespace geopter

#endif // PUPILSPEC_H

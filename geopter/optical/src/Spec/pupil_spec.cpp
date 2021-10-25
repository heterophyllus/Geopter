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
/// \file   pupil_spec.cpp
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================

#include <iostream>
#include <iomanip>

#include "spec/pupil_spec.h"

using namespace geopter;

PupilSpec::PupilSpec() :
    pupil_type_(PupilType::EPD),
    value_(10.0)
{

}


PupilSpec::PupilSpec(int pupil_type, double value) :
    pupil_type_(pupil_type),
    value_(value)
{

}

PupilSpec::~PupilSpec()
{

}


void PupilSpec::print(std::ostringstream &oss)
{
    constexpr int label_w = 15;

    oss << std::setw(label_w) << std::left << "Type: ";
    switch (pupil_type_) {
    case PupilType::EPD:
        oss << "Entrance Pupil Diameter";
        break;
    case PupilType::FNO:
        oss << "F-Number";
        break;
    case PupilType::NA:
        oss << "Image Space NA(Numerical Aperture)";
        break;
    case PupilType::NAO:
        oss << "Object Space NA(Numerical Aperture)";
        break;
    }
    oss << std::endl;


    oss << std::setw(label_w) << std::left << "Value: ";
    oss << value_ << std::endl;
    oss << std::endl;

}

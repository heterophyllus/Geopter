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
/// \file   first_order.cpp
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#include <iostream>
#include <iomanip>

#include "paraxial/first_order.h"

using namespace geopter;

void FirstOrderData::print()
{
    std::ostringstream oss;
    print(oss);
    std::cout << oss.str() << std::endl;
}

void FirstOrderData::print(std::ostringstream& oss)
{
    constexpr int fixed_w = 20;
    constexpr int pre = 4;

    oss << std::setw(fixed_w) << std::left << "efl";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << efl << std::endl;

    oss << std::setw(fixed_w) << std::left << "ffl";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << ffl << std::endl;

    oss << std::setw(fixed_w) << std::left << "bfl";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << bfl << std::endl;

    oss << std::setw(fixed_w) << std::left << "f/#";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << fno << std::endl;

    oss << std::setw(fixed_w) << std::left << "red";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << red << std::endl;

    oss << std::setw(fixed_w) << std::left << "obj_dist";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << obj_dist << std::endl;

    oss << std::setw(fixed_w) << std::left << "obj_ang";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << obj_ang << std::endl;

    oss << std::setw(fixed_w) << std::left << "enp_dist";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << enp_dist << std::endl;

    oss << std::setw(fixed_w) << std::left << "enp_radius";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << enp_radius << std::endl;

    oss << std::setw(fixed_w) << std::left << "img_dist";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << img_dist << std::endl;

    oss << std::setw(fixed_w) << std::left << "img_ht";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << img_ht << std::endl;

    oss << std::setw(fixed_w) << std::left << "exp_dist";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << exp_dist << std::endl;

    oss << std::setw(fixed_w) << std::left << "exp_radius";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << exp_radius << std::endl;

    oss << std::setw(fixed_w) << std::left << "opt inv";
    oss << std::setw(fixed_w) << std::right << std::fixed << std::setprecision(pre) << opt_inv << std::endl;

    oss << std::endl;
}

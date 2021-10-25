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
/// \file   geopter_error.cpp
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#include "common/geopter_error.h"

using namespace geopter;

OutOfRangeError::OutOfRangeError() :
    std::out_of_range("out of range error"),
    array_name_("Unknown array"),
    array_size_(0),
    access_index_(0)
{

}

OutOfRangeError::~OutOfRangeError()
{

}

std::string OutOfRangeError::what_str() const
{
    std::string str =  array_name_ + " out of range error: " + "(access to " + std::to_string(access_index_) + " in size " + std::to_string(array_size_);
    return str;
}

std::string OutOfRangeError::array_name() const
{
    return array_name_;
}

int OutOfRangeError::array_size() const
{
    return array_size_;
}

int OutOfRangeError::access_index() const
{
    return access_index_;
}

void OutOfRangeError::set_array_name(const std::string &name)
{
    array_name_ = name;
}

void OutOfRangeError::set_array_size(int n)
{
    array_size_ = n;
}

void OutOfRangeError::set_access_index(int i)
{
    access_index_ = i;
}


//**********************************************************************************************************

RayOutOfRangeError::RayOutOfRangeError() : OutOfRangeError(),
    ray_(nullptr)
{
    array_name_ = "Ray";
}

RayOutOfRangeError::~RayOutOfRangeError()
{
    ray_.reset();
}

std::shared_ptr<Ray> RayOutOfRangeError::ray() const
{
    return ray_;
}

void RayOutOfRangeError::set_ray(std::shared_ptr<Ray> ray)
{
    ray_ = ray;
}

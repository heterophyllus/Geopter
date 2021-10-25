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

//============================================================================
/// \file   geopter_error.h
/// \author Hiiragi
/// \date   October 24th, 2021
/// \brief  
//============================================================================


#ifndef GEOPTER_ERROR_H
#define GEOPTER_ERROR_H

#include <exception>
#include <stdexcept>
#include <string>
#include <memory>


namespace geopter {

class Ray;

class OutOfRangeError : public std::out_of_range
{
public:
    OutOfRangeError();
    virtual ~OutOfRangeError();

    virtual std::string what_str() const;

    std::string array_name() const;
    int array_size() const;
    int access_index() const;

    void set_array_name(const std::string& name);
    void set_array_size(int n);
    void set_access_index(int i);

protected:
    std::string array_name_;
    int array_size_;
    int access_index_;
};


class RayOutOfRangeError : public OutOfRangeError
{
public:
    RayOutOfRangeError();
    ~RayOutOfRangeError();

    std::shared_ptr<Ray> ray() const;

    void set_ray(std::shared_ptr<Ray> ray);

private:
    std::shared_ptr<Ray> ray_;
};


}

#endif

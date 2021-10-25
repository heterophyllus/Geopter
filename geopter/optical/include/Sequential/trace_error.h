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
/// \file   trace_error.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#ifndef TRACEERROR_H
#define TRACEERROR_H

#include "ray.h"

namespace geopter {

class Surface;

class TraceError : public std::exception
{
public:
    TraceError();
    virtual ~TraceError();

    virtual std::string cause_str() const;
    std::shared_ptr<Ray> ray() const;
    int surface_index() const;

    void set_ray(std::shared_ptr<Ray> ray);
    void set_surface(Surface* s);
    void set_surface_index(int i);

protected:
    std::string cause_str_;
    std::shared_ptr<Ray> ray_;
    Surface* caused_surface_;
    int surface_index_;
};


class TraceTIRError : public TraceError
{
public:
    TraceTIRError();
};

class TraceMissedSurfaceError : public TraceError
{
public:
    TraceMissedSurfaceError();
};


class TraceBlockedByApertureError : public TraceError
{
public:
    TraceBlockedByApertureError();
};

class TraceRayAimingFailedError : public TraceError
{
public:
    TraceRayAimingFailedError();
};

} //namespace geopter

#endif // TRACEERROR_H

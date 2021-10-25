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
/// \file   trace_error.cpp
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  
//============================================================================


#include "sequential/trace_error.h"

using namespace geopter;

TraceError::TraceError() :
    caused_surface_(nullptr),
    surface_index_(0)
{
    cause_str_ = "Unnown trace error";
}

TraceError::~TraceError()
{
    caused_surface_ = nullptr;
}

std::string TraceError::cause_str() const
{
    return cause_str_;
}

std::shared_ptr<Ray> TraceError::ray() const
{
    return ray_;
}

int TraceError::surface_index() const
{
    return surface_index_;
}

void TraceError::set_ray(std::shared_ptr<Ray> ray)
{
    ray_ = ray;
}

void TraceError::set_surface(Surface *s)
{
    caused_surface_ = s;
}

void TraceError::set_surface_index(int i)
{
    surface_index_ = i;
}


//**********************************************************
TraceTIRError::TraceTIRError()
{
    cause_str_ = "Total reflection";
}

//**********************************************************
TraceMissedSurfaceError::TraceMissedSurfaceError()
{
    cause_str_ = "Ray missed surface";
}

//**********************************************************
TraceBlockedByApertureError::TraceBlockedByApertureError()
{
    cause_str_ = "Ray blocked by aperture";
}

//**********************************************************
TraceRayAimingFailedError::TraceRayAimingFailedError()
{
    cause_str_ = "Ray aiming failed";
}

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
/// \file   optical.h
/// \author Hiiragi
/// \date   September 12th, 2021
/// \brief  include all header files 
//============================================================================


#ifndef OPTICAL_H
#define OPTICAL_H

#ifdef _MSC_VER
    #include "msdirent.h"
#else
    #include <dirent.h>
#endif

#include <memory>
#include <vector>


#include "analysis/aberration.h"
#include "analysis/layout.h"
//#include "Analysis/ray_fan.h"
//#include "Analysis/longitudinal.h"

#include "assembly/optical_assembly.h"
#include "assembly/aperture.h"
#include "assembly/circular.h"
#include "assembly/interface.h"
#include "assembly/surface.h"
#include "assembly/gap.h"

#include "profile/surface_profile.h"
#include "profile/spherical.h"
#include "profile/even_polynomial.h"

#include "material/material_library.h"
#include "material/material.h"
#include "material/glass.h"
#include "material/glass_catalog.h"
#include "material/dispersion_formula.h"
#include "material/air.h"
#include "material/buchdahl_glass.h"

#include "spec/optical_spec.h"
#include "spec/field_spec.h"
#include "spec/field.h"
#include "spec/pupil_spec.h"
#include "spec/wvl_spec.h"
#include "spec/field.h"
#include "spec/wvl.h"
#include "spec/spectral_line.h"

#include "system/optical_system.h"

#include "paraxial/paraxial_path.h"
#include "paraxial/paraxial_ray.h"
#include "paraxial/paraxial_trace.h"

#include "sequential/sequential_path.h"
#include "sequential/sequential_trace.h"
#include "sequential/ray.h"
#include "sequential/trace_error.h"

#include "element/element.h"
#include "element/lens.h"
#include "element/mirror.h"
#include "element/dummy_interface.h"
#include "element/stop.h"
#include "element/air_gap.h"
#include "element/element_model.h"

#include "renderer/renderer.h"
#include "renderer/rgb.h"

#include "utility/utility.h"

#include "fileio/file_io.h"

#include "environment/environment.h"

#endif // OPTICAL_H

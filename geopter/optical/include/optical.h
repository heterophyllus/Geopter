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


#ifndef OPTICAL_H
#define OPTICAL_H

#ifdef _MSC_VER
    #include "msdirent.h"
#else
    #include <dirent.h>
#endif

#include "system/optical_system.h"

#include "analysis/layout.h"
#include "analysis/astigmatism.h"
#include "analysis/spherochromatism.h"
#include "analysis/chromatic_focus_shift.h"
#include "analysis/spot_diagram.h"
#include "analysis/transverse_ray_fan.h"
#include "analysis/opd_fan.h"
#include "analysis/wavefront.h"
#include "analysis/diffractive_psf.h"
#include "analysis/geometrical_mtf.h"
#include "analysis/diffractive_mtf.h"

#include "assembly/optical_assembly.h"

#include "solve/fixed_solve.h"
#include "solve/edge_thickness_solve.h"
#include "solve/overall_length_solve.h"
#include "solve/marginal_ray_height_solve.h"

#include "material/material_library.h"
#include "material/buchdahl_glass.h"

#include "spec/optical_spec.h"
#include "spec/spectral_line.h"

#include "paraxial/paraxial_trace.h"

#include "sequential/sequential_trace.h"
#include "sequential/ray.h"
#include "sequential/trace_error.h"

#include "element/lens.h"
#include "element/mirror.h"
#include "element/dummy_interface.h"
#include "element/stop.h"
#include "element/air_gap.h"
#include "element/element_model.h"

#include "renderer/renderer.h"
//#include "renderer/renderer_gnuplot.h"
#include "renderer/rgb.h"

#include "common/string_tool.h"

#include "environment/environment.h"

#endif // OPTICAL_H

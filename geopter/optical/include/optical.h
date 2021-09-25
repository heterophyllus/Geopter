#ifndef OPTICAL_H
#define OPTICAL_H

#ifdef _MSC_VER
    #include "msdirent.h"
#else
    #include <dirent.h>
#endif

#include <memory>
#include <vector>

/** include all header files **/
#include "Analysis/aberration.h"
#include "Analysis/layout.h"
//#include "Analysis/ray_fan.h"
//#include "Analysis/longitudinal.h"

#include "Assembly/optical_assembly.h"
#include "Assembly/aperture.h"
#include "Assembly/circular.h"
#include "Assembly/interface.h"
#include "Assembly/surface.h"
#include "Assembly/surface_profile.h"
#include "Assembly/spherical.h"
#include "Assembly/even_polynomial.h"
#include "Assembly/gap.h"

#include "Material/material_library.h"
#include "Material/material.h"
#include "Material/glass.h"
#include "Material/glass_catalog.h"
#include "Material/dispersion_formula.h"
#include "Material/air.h"
#include "Material/buchdahl_glass.h"

#include "Spec/optical_spec.h"
#include "Spec/field_spec.h"
#include "Spec/field.h"
#include "Spec/pupil_spec.h"
#include "Spec/wvl_spec.h"
#include "Spec/field.h"
#include "Spec/wvl.h"
#include "Spec/spectral_line.h"

#include "System/optical_system.h"

#include "Paraxial/paraxial_path.h"
#include "Paraxial/paraxial_ray.h"
#include "Paraxial/paraxial_trace.h"

#include "Sequential/sequential_path.h"
#include "Sequential/sequential_trace.h"
#include "Sequential/ray.h"
#include "Sequential/trace_error.h"

#include "Element/element.h"
#include "Element/lens.h"
#include "Element/mirror.h"
#include "Element/dummy_interface.h"
#include "Element/stop.h"
#include "Element/air_gap.h"
#include "Element/element_model.h"

#include "Renderer/renderer.h"
#include "Renderer/rgb.h"

#include "Utility/utility.h"

#include "FileIO/file_io.h"


#endif // OPTICAL_H

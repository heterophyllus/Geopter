
#project(GeopterOptical LANGUAGES CXX)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(OPTICAL_SRCS 
    Analysis/aberration.cpp
    Analysis/ray_fan.cpp
    Analysis/longitudinal.cpp
    Analysis/layout.cpp

    Assembly/optical_assembly.cpp
    
    #Assembly/transformation.cpp
    Assembly/interface.cpp
    Assembly/surface.cpp
    Assembly/surface_profile.cpp
    Assembly/spherical.cpp
    Assembly/even_polynomial.cpp
    Assembly/aperture.cpp
    Assembly/circular.cpp
    Assembly/gap.cpp
    Assembly/decenter_data.cpp

    Element/element_model.cpp
    Element/element.cpp
    Element/lens.cpp
    Element/mirror.cpp
    Element/dummy_interface.cpp
    Element/stop.cpp
    Element/air_gap.cpp

    Spec/spectral_line.cpp
    Spec/optical_spec.cpp
    Spec/pupil_spec.cpp
    Spec/field_spec.cpp
    Spec/field.cpp
    Spec/wvl_spec.cpp
    Spec/wvl.cpp

    FileIO/file_io.cpp

    Material/material_library.cpp
    Material/glass_catalog.cpp
    Material/buchdahl_glass.cpp
    Material/material.cpp
    Material/air.cpp
    Material/glass.cpp

    System/optical_system.cpp

    #Paraxial/paraxial_data.cpp
    Paraxial/paraxial_ray.cpp
    Paraxial/paraxial_path.cpp
    Paraxial/first_order.cpp
    Paraxial/paraxial_trace.cpp

    #Sequential/sequential_data.cpp
    Sequential/sequential_path.cpp

    Sequential/ray.cpp
    #Sequential/trace.cpp
    Sequential/trace_error.cpp
    Sequential/sequential_trace.cpp

    Renderer/renderer.cpp
    #Renderer/renderer_svg.cpp
    Renderer/rgb.cpp

    Utility/utility.cpp
)


add_library(geopteroptical STATIC ${OPTICAL_SRCS})

target_include_directories(geopteroptical PUBLIC ${CMAKE_SOURCE_DIR}../include)

install(TARGETS geopteroptical LIBRARY DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/optical)

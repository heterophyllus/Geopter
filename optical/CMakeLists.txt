cmake_minimum_required(VERSION 3.5)

project(GeopterOptical LANGUAGES CXX VERSION ${VERSION_SHORT})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
if(MSVC)
    set(BUILD_STATIC ON)
endif()
if(BUILD_STATIC)
    set(CMAKE_STATIC_LIBRARY_SUFFIX "_static${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()


set(OPTICAL_SRCS
    file_io.cpp
    database.cpp
    spectral_line.cpp
    glass_catalog.cpp
    buchdahl_glass.cpp
    optical_model.cpp
    sequential_model.cpp
    paraxial_model.cpp
    paraxial_ray.cpp
    first_order.cpp
    element_model.cpp
    element.cpp
    optical_spec.cpp
    pupil_spec.cpp
    field_spec.cpp
    field.cpp
    wvl_spec.cpp
    wvl.cpp
    transform.cpp
    interface.cpp
    surface.cpp
    surface_profile.cpp
    spherical.cpp
    even_polynomial.cpp
    aperture.cpp
    circular.cpp
    medium.cpp
    air.cpp
    glass.cpp
    gap.cpp
    decenter_data.cpp
    ray.cpp
    ray_fan.cpp
    longitudinal.cpp
    trace.cpp
    trace_error.cpp
    layout.cpp
    renderer.cpp
    renderer_svg.cpp
    rgb.cpp
)

# test
add_subdirectory(test)

if(BUILD_STATIC)
    add_library(GeopterOptical STATIC ${OPTICAL_SRCS})
    target_compile_definitions(GeopterOptical PUBLIC GeopterOptical_STATIC)
else()
    add_library(GeopterOptical SHARED ${OPTICAL_SRCS})
    target_compile_definitions(GeopterOptical PRIVATE GeopterOptical_SHARED_EXPORT)
endif()

set_target_properties(GeopterOptical
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib>
    LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib>
    RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>
    )

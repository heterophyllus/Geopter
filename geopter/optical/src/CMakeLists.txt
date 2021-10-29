
project(geopter-optical)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


set(OPTICAL_SRCS 
    analysis/aberration.cpp
    #analysis/ray_fan.cpp
    #analysis/longitudinal.cpp
    analysis/layout.cpp

    assembly/optical_assembly.cpp
    assembly/interface.cpp
    assembly/surface.cpp
    assembly/aperture.cpp
    assembly/circular.cpp
    assembly/gap.cpp
    assembly/decenter_data.cpp

    profile/surface_profile.cpp
    profile/spherical.cpp
    profile/even_polynomial.cpp
    profile/odd_polynomial.cpp

    element/element_model.cpp
    element/element.cpp
    element/lens.cpp
    element/mirror.cpp
    element/dummy_interface.cpp
    element/stop.cpp
    element/air_gap.cpp

    environment/environment.cpp

    spec/spectral_line.cpp
    spec/optical_spec.cpp
    spec/pupil_spec.cpp
    spec/field_spec.cpp
    spec/field.cpp
    spec/wvl_spec.cpp
    spec/wvl.cpp

    #fileio/file_io.cpp

    material/material_library.cpp
    material/glass_catalog.cpp
    material/buchdahl_glass.cpp
    material/material.cpp
    material/air.cpp
    material/glass.cpp

    system/optical_system.cpp

    paraxial/paraxial_ray.cpp
    paraxial/paraxial_path.cpp
    paraxial/first_order.cpp
    paraxial/paraxial_trace.cpp

    sequential/sequential_path.cpp
    sequential/ray.cpp
    sequential/trace_error.cpp
    sequential/sequential_trace.cpp

    renderer/renderer.cpp
    #renderer/renderer_svg.cpp
    renderer/rgb.cpp

    data/plot_data.cpp
    data/point_set.cpp

    common/geopter_error.cpp


    utility/utility.cpp
)


add_library(${PROJECT_NAME} STATIC
    ${OPTICAL_SRCS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/geopter/optical/include
    ${CMAKE_SOURCE_DIR}/3rdparty #Eigen, nlohman
    ${CMAKE_SOURCE_DIR}/3rdparty/spline/src
)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/optical)

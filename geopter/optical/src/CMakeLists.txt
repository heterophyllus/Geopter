
project(geopter-optical)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


set(OPTICAL_SRCS 
    analysis/ray_aberration.cpp
    analysis/transverse_ray_fan.cpp
    analysis/spherochromatism.cpp
    analysis/astigmatism.cpp
    analysis/chromatic_focus_shift.cpp
    analysis/spot_diagram.cpp
    analysis/layout.cpp
    analysis/wave_aberration.cpp
    analysis/reference_sphere.cpp
    analysis/opd_fan.cpp
    analysis/wavefront.cpp
    analysis/diffractive_psf.cpp

    assembly/optical_assembly.cpp
    assembly/interface.cpp
    assembly/surface.cpp
    assembly/aperture.cpp
    assembly/circular.cpp
    assembly/gap.cpp
    assembly/decenter_data.cpp
    assembly/transformation.cpp

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


    material/material_library.cpp
    material/glass_catalog.cpp
    material/buchdahl_glass.cpp
    material/material.cpp
    material/air.cpp
    material/glass.cpp

    system/optical_system.cpp
    system/paraxial_data.cpp

    paraxial/paraxial_ray.cpp
    paraxial/paraxial_path.cpp
    paraxial/paraxial_trace.cpp

    sequential/sequential_path.cpp
    sequential/ray.cpp
    sequential/ray_at_surface.cpp
    sequential/trace_error.cpp
    sequential/sequential_trace.cpp

    renderer/renderer.cpp
    renderer/renderer_mpl.cpp
    renderer/rgb.cpp

    data/plot_data.cpp
    data/graph2d.cpp
    data/map_data_3d.cpp
    #data/grid_array.cpp

    common/geopter_error.cpp


    utility/utility.cpp
)


add_library(${PROJECT_NAME} STATIC
    ${OPTICAL_SRCS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/geopter/optical/include
    ${CMAKE_SOURCE_DIR}/3rdparty #nlohman
    ${CMAKE_SOURCE_DIR}/3rdparty/spline/src
    ${CMAKE_SOURCE_DIR}/3rdparty/eigen-3.3.9
)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/optical)

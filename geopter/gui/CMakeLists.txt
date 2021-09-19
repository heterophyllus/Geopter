cmake_minimum_required(VERSION 3.5)


set(PROJECT_SOURCES
    main.cpp

    main_window.h
    main_window.cpp
    main_window.ui

    lens_data_manager_dock.h
    lens_data_manager_dock.cpp

    general_configuration_dialog.cpp
    general_configuration_dialog.h
    general_configuration_dialog.ui

    float_delegate.h

    text_view_dock.h
    text_view_dock.cpp

    plot_view_dock.h
    plot_view_dock.cpp

    qdebugstream.h

    renderer_qcp.h
    renderer_qcp.cpp

    spec_setting_dialog.h
    spec_setting_dialog.cpp
    spec_setting_dialog.ui

    surface_property_dialog.cpp
    surface_property_dialog.h
    surface_property_dialog.ui

    layout_dialog.cpp
    layout_dialog.h
    layout_dialog.ui

    single_ray_trace_dialog.h
    single_ray_trace_dialog.cpp
    single_ray_trace_dialog.ui

    paraxial_trace_dialog.h
    paraxial_trace_dialog.cpp
    paraxial_trace_dialog.ui

    transverse_ray_fan_dialog.cpp
    transverse_ray_fan_dialog.h
    transverse_ray_fan_dialog.ui

    longitudinal_setting_dialog.cpp
    longitudinal_setting_dialog.h
    longitudinal_setting_dialog.ui
)

add_executable( Geopter ${PROJECT_SOURCES} )

target_include_directories(Geopter PRIVATE 3rdparty/Qt-Advanced-Docking)
target_link_libraries(Geopter PRIVATE Qt5::Widgets Qt5::PrintSupport QCustomPlot)
target_link_libraries(Geopter PRIVATE qtadvanceddocking)

target_include_directories(Geopter PRIVATE ${CMAKE_SOURCE_DIR}../optical/include)
target_link_libraries(Geopter PRIVATE GeopterOptical)


# surpress console window
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()



# Collect all Qt dependencies

if(WIN32)
    # Retrieve the absolute path to qmake and then use that path to find
    # the binaries
    get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
    #find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
    add_custom_command(TARGET Geopter POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E
            env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
                "$<TARGET_FILE:Geopter>"
        COMMENT "Running windeployqt..."
    )
elseif(APPLE)
    get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    #find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
    add_custom_command(TARGET Geopter POST_BUILD
        COMMAND "${MACDEPLOYQT_EXECUTABLE}"
            "$<TARGET_FILE:Geopter>"
            -always-overwrite
        COMMENT "Running macdeployqt..."
    )
endif()

cmake_minimum_required(VERSION 3.5)


set(PROJECT_SOURCES
    main.cpp

    main_window.h
    main_window.cpp
    main_window.ui

    text_view_dock.h
    text_view_dock.cpp

    plot_view_dock.h
    plot_view_dock.cpp

    qdebugstream.h

    general_configuration_dialog.cpp
    general_configuration_dialog.h
    general_configuration_dialog.ui

    SystemEditor/system_editor_dock.cpp
    SystemEditor/system_editor_dock.h
    SystemEditor/system_editor_widget.h
    SystemEditor/system_editor_widget.cpp
    SystemEditor/system_editor_widget.ui
    SystemEditor/surface_property_dialog.cpp
    SystemEditor/surface_property_dialog.h
    SystemEditor/surface_property_dialog.ui
    SystemEditor/float_delegate.h

    Analysis/renderer_qcp.h
    Analysis/renderer_qcp.cpp

    Analysis/analysis_setting_dialog.cpp
    Analysis/analysis_setting_dialog.h

    Analysis/prescription_setting_dialog.cpp
    Analysis/prescription_setting_dialog.h
    Analysis/prescription_setting_dialog.ui

    Analysis/layout_dialog.cpp
    Analysis/layout_dialog.h
    Analysis/layout_dialog.ui

    Analysis/single_ray_trace_dialog.h
    Analysis/single_ray_trace_dialog.cpp
    Analysis/single_ray_trace_dialog.ui

    Analysis/paraxial_trace_dialog.h
    Analysis/paraxial_trace_dialog.cpp
    Analysis/paraxial_trace_dialog.ui

    Analysis/transverse_ray_fan_dialog.cpp
    Analysis/transverse_ray_fan_dialog.h
    Analysis/transverse_ray_fan_dialog.ui

    Analysis/field_curvature_setting_dialog.cpp
    Analysis/field_curvature_setting_dialog.h
    Analysis/field_curvature_setting_dialog.ui

    Analysis/longitudinal_setting_dialog.cpp
    Analysis/longitudinal_setting_dialog.h
    Analysis/longitudinal_setting_dialog.ui

    Analysis/chromatic_focusshift_setting_dialog.cpp
    Analysis/chromatic_focusshift_setting_dialog.h
    Analysis/chromatic_focusshift_setting_dialog.ui

    Analysis/spot_diagram_setting_dialog.cpp
    Analysis/spot_diagram_setting_dialog.h
    Analysis/spot_diagram_setting_dialog.ui
)

add_executable( Geopter ${PROJECT_SOURCES} )

target_include_directories(Geopter PRIVATE 3rdparty/Qt-Advanced-Docking)
target_link_libraries(Geopter PRIVATE Qt5::Widgets Qt5::PrintSupport QCustomPlot)
target_link_libraries(Geopter PRIVATE qtadvanceddocking)

target_include_directories(Geopter PRIVATE ${CMAKE_SOURCE_DIR}../optical/include)
target_link_libraries(Geopter PRIVATE geopteroptical)


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

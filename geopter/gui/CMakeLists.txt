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

    renderer_qcp.h
    renderer_qcp.cpp

    qdebugstream.h

    general_configuration_dialog.cpp
    general_configuration_dialog.h
    general_configuration_dialog.ui

    systemeditor/system_editor_dock.cpp
    systemeditor/system_editor_dock.h
    systemeditor/system_editor_widget.h
    systemeditor/system_editor_widget.cpp
    systemeditor/system_editor_widget.ui
    systemeditor/surface_property_dialog.cpp
    systemeditor/surface_property_dialog.h
    systemeditor/surface_property_dialog.ui
    systemeditor/float_delegate.h

    analysisdialogs/analysis_setting_dialog.cpp
    analysisdialogs/analysis_setting_dialog.h

    analysisdialogs/prescription_setting_dialog.cpp
    analysisdialogs/prescription_setting_dialog.h
    analysisdialogs/prescription_setting_dialog.ui

    analysisdialogs/layout_dialog.cpp
    analysisdialogs/layout_dialog.h
    analysisdialogs/layout_dialog.ui

    analysisdialogs/single_ray_trace_dialog.h
    analysisdialogs/single_ray_trace_dialog.cpp
    analysisdialogs/single_ray_trace_dialog.ui

    analysisdialogs/paraxial_trace_dialog.h
    analysisdialogs/paraxial_trace_dialog.cpp
    analysisdialogs/paraxial_trace_dialog.ui

    analysisdialogs/transverse_ray_fan_dialog.cpp
    analysisdialogs/transverse_ray_fan_dialog.h
    analysisdialogs/transverse_ray_fan_dialog.ui

    analysisdialogs/field_curvature_setting_dialog.cpp
    analysisdialogs/field_curvature_setting_dialog.h
    analysisdialogs/field_curvature_setting_dialog.ui

    analysisdialogs/longitudinal_setting_dialog.cpp
    analysisdialogs/longitudinal_setting_dialog.h
    analysisdialogs/longitudinal_setting_dialog.ui

    analysisdialogs/chromatic_focusshift_setting_dialog.cpp
    analysisdialogs/chromatic_focusshift_setting_dialog.h
    analysisdialogs/chromatic_focusshift_setting_dialog.ui

    analysisdialogs/spot_diagram_setting_dialog.cpp
    analysisdialogs/spot_diagram_setting_dialog.h
    analysisdialogs/spot_diagram_setting_dialog.ui
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

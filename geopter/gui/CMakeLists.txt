cmake_minimum_required(VERSION 3.5)


set(GEOPTER_GUI_SOURCES
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

    ${CMAKE_SOURCE_DIR}/3rdparty/pythonqt/src/gui/PythonQtScriptingConsole.h
    ${CMAKE_SOURCE_DIR}/3rdparty/pythonqt/src/gui/PythonQtScriptingConsole.cpp
)

add_executable(Geopter ${GEOPTER_GUI_SOURCES})

target_include_directories(Geopter PRIVATE
    ${CMAKE_SOURCE_DIR}/3rdparty/Qt-Advanced-Docking
    ${CMAKE_SOURCE_DIR}/3rdparty/QCustomPlot
    ${CMAKE_SOURCE_DIR}/3rdparty/pythonqt/src
    ${CMAKE_SOURCE_DIR}/3rdparty/pythonqt/extensions/PythonQt_QtAll
    ${CMAKE_SOURCE_DIR}/3rdparty #Eigen
    ${CMAKE_SOURCE_DIR}/3rdparty/spline/src
    ${CMAKE_SOURCE_DIR}/geopter/optical/include
    ${Python_INCLUDE_DIRS}
)

target_compile_definitions(Geopter PRIVATE
    PYTHONQT_QTALL_EXPORTS
    PYTHONQT_WITH_CORE
    PYTHONQT_WITH_GUI
    PYTHONQT_WITH_SVG
    PYTHONQT_WITH_OPENGL
    PYTHONQT_WITH_MULTIMEDIA
    PYTHONQT_WITH_UITOOLS
)

target_link_libraries(Geopter PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::PrintSupport
    Qt5::Multimedia
    Qt5::MultimediaWidgets
    Qt5::OpenGL
    #Qt5::Sql
    Qt5::Svg
    #Qt5::Xml
    #Qt5::XmlPatterns
    Qt5::UiTools
    qtadvanceddocking
    QCustomPlot
    geopteroptical
    PythonQt
    PythonQt_QtAll
    ${Python_LIBRARIES}
)


# surpress console window
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()


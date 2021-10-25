cmake_minimum_required(VERSION 3.5)

project(geopter-gui)

set(GEOPTER_GUI_HEADERS
    MainWindow.h
    TextViewDock.h
    PlotViewDock.h
    renderer_qcp.h
    qdebugstream.h
    GeneralConfigDlg.h

    SystemEditor/SystemEditorDock.h
    SystemEditor/SystemEditorWidget.h
    SystemEditor/SurfacePropertyDlg.h
    SystemEditor/float_delegate.h

    AnalysisDlg/AnalysisSettingDlg.h
    AnalysisDlg/PrescriptionDlg.h
    AnalysisDlg/Layout2dDlg.h
    AnalysisDlg/SingleRayTraceDlg.h
    AnalysisDlg/ParaxialTraceDlg.h
    AnalysisDlg/TransverseRayFanDlg.h
    AnalysisDlg/FieldCurvatureDlg.h
    AnalysisDlg/LongitudinalDlg.h
    AnalysisDlg/ChromaticFocusShiftDlg.h
    AnalysisDlg/SpotDiagramDlg.h

    ${CMAKE_SOURCE_DIR}/3rdparty/pythonqt/src/gui/PythonQtScriptingConsole.h
)


set(GEOPTER_GUI_SOURCES
    main.cpp

    MainWindow.cpp

    TextViewDock.cpp
    PlotViewDock.cpp

    renderer_qcp.cpp

    GeneralConfigDlg.cpp

    SystemEditor/SystemEditorDock.cpp
    SystemEditor/SystemEditorWidget.cpp
    SystemEditor/SurfacePropertyDlg.cpp

    AnalysisDlg/AnalysisSettingDlg.cpp
    AnalysisDlg/PrescriptionDlg.cpp
    AnalysisDlg/Layout2dDlg.cpp
    AnalysisDlg/SingleRayTraceDlg.cpp
    AnalysisDlg/ParaxialTraceDlg.cpp
    AnalysisDlg/TransverseRayFanDlg.cpp
    AnalysisDlg/FieldCurvatureDlg.cpp
    AnalysisDlg/LongitudinalDlg.cpp
    AnalysisDlg/ChromaticFocusShiftDlg.cpp
    AnalysisDlg/SpotDiagramDlg.cpp

    ${CMAKE_SOURCE_DIR}/3rdparty/pythonqt/src/gui/PythonQtScriptingConsole.cpp
)

set(GEOPTER_GUI_FORMS
    MainWindow.ui

    GeneralConfigDlg.ui
    SystemEditor/SystemEditorWidget.ui
    SystemEditor/SurfacePropertyDlg.ui

    AnalysisDlg/PrescriptionDlg.ui
    AnalysisDlg/Layout2dDlg.ui
    AnalysisDlg/SingleRayTraceDlg.ui
    AnalysisDlg/SpotDiagramDlg.ui
    AnalysisDlg/ChromaticFocusShiftDlg.ui
    AnalysisDlg/LongitudinalDlg.ui
    AnalysisDlg/ParaxialTraceDlg.ui
    AnalysisDlg/TransverseRayFanDlg.ui
    AnalysisDlg/FieldCurvatureDlg.ui
)

add_executable(${PROJECT_NAME} ${GEOPTER_GUI_SOURCES} ${GEOPTER_GUI_HEADERS} ${GEOPTER_GUI_FORMS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/3rdparty/Qt-Advanced-Docking
    ${CMAKE_SOURCE_DIR}/3rdparty/QCustomPlot
    ${CMAKE_SOURCE_DIR}/3rdparty/pythonqt/src
    ${CMAKE_SOURCE_DIR}/3rdparty/pythonqt/extensions/PythonQt_QtAll
    ${CMAKE_SOURCE_DIR}/3rdparty #Eigen
    ${CMAKE_SOURCE_DIR}/3rdparty/spline/src
    ${CMAKE_SOURCE_DIR}/geopter/optical/include
    ${Python_INCLUDE_DIRS}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    PYTHONQT_QTALL_EXPORTS
    PYTHONQT_WITH_CORE
    PYTHONQT_WITH_GUI
    PYTHONQT_WITH_SVG
    PYTHONQT_WITH_OPENGL
    PYTHONQT_WITH_MULTIMEDIA
    PYTHONQT_WITH_UITOOLS
)

target_link_libraries(${PROJECT_NAME} PRIVATE
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
    geopter-optical
    PythonQt
    PythonQt_QtAll
    ${Python_LIBRARIES}
)



# surpress console window
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

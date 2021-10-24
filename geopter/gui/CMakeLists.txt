cmake_minimum_required(VERSION 3.5)


set(GEOPTER_GUI_SOURCES
    main.cpp

    MainWindow.h
    MainWindow.cpp
    MainWindow.ui

    TextViewDock.h
    TextViewDock.cpp

    PlotViewDock.h
    PlotViewDock.cpp

    renderer_qcp.h
    renderer_qcp.cpp

    qdebugstream.h

    GeneralConfigDlg.cpp
    GeneralConfigDlg.h
    GeneralConfigDlg.ui

    SystemEditor/SystemEditorDock.cpp
    SystemEditor/SystemEditorDock.h
    SystemEditor/SystemEditorWidget.h
    SystemEditor/SystemEditorWidget.cpp
    SystemEditor/SystemEditorWidget.ui
    SystemEditor/SurfacePropertyDlg.cpp
    SystemEditor/SurfacePropertyDlg.h
    SystemEditor/SurfacePropertyDlg.ui
    SystemEditor/float_delegate.h

    AnalysisDlg/AnalysisSettingDlg.cpp
    AnalysisDlg/AnalysisSettingDlg.h

    AnalysisDlg/PrescriptionDlg.cpp
    AnalysisDlg/PrescriptionDlg.h
    AnalysisDlg/PrescriptionDlg.ui

    AnalysisDlg/Layout2dDlg.cpp
    AnalysisDlg/Layout2dDlg.h
    AnalysisDlg/Layout2dDlg.ui

    AnalysisDlg/SingleRayTraceDlg.h
    AnalysisDlg/SingleRayTraceDlg.cpp
    AnalysisDlg/SingleRayTraceDlg.ui

    AnalysisDlg/ParaxialTraceDlg.h
    AnalysisDlg/ParaxialTraceDlg.cpp
    AnalysisDlg/ParaxialTraceDlg.ui

    AnalysisDlg/TransverseRayFanDlg.cpp
    AnalysisDlg/TransverseRayFanDlg.h
    AnalysisDlg/TransverseRayFanDlg.ui

    AnalysisDlg/FieldCurvatureDlg.cpp
    AnalysisDlg/FieldCurvatureDlg.h
    AnalysisDlg/FieldCurvatureDlg.ui

    AnalysisDlg/LongitudinalDlg.cpp
    AnalysisDlg/LongitudinalDlg.h
    AnalysisDlg/LongitudinalDlg.ui

    AnalysisDlg/ChromaticFocusShiftDlg.cpp
    AnalysisDlg/ChromaticFocusShiftDlg.h
    AnalysisDlg/ChromaticFocusShiftDlg.ui

    AnalysisDlg/SpotDiagramDlg.cpp
    AnalysisDlg/SpotDiagramDlg.h
    AnalysisDlg/SpotDiagramDlg.ui

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


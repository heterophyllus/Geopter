cmake_minimum_required(VERSION 3.5)

project(geopter-gui)


## ***************************************************************************************
## ** Qt
## ***************************************************************************************
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# If cmake raises "QT_DIR not found" error, set Qt install path explicitly.
# set(CMAKE_PREFIX_PATH "C:/Qt/(version)/(kit)")

find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport Multimedia MultimediaWidgets OpenGL SVG uitools REQUIRED)


## ***************************************************************************************
## ** Python
## ***************************************************************************************
#set(Python_ADDITIONAL_VERSIONS 3.9)
find_package(Python COMPONENTS Interpreter Development REQUIRED)
message(STATUS "Python_FOUND= ${Python_FOUND}")
message(STATUS "Python_VERSION= ${Python_VERSION}")
message(STATUS "Python_INCLUDE_DIRS= ${Python_INCLUDE_DIRS}")
message(STATUS "Python_LIBRARIES= ${Python_LIBRARIES}")
message(STATUS "Python_NumPy_FOUND= ${Python_NumPy_FOUND}")


set(GEOPTER_GUI_HEADERS
    MainWindow.h

    OpticalSystemWrapper/QOpticalSystem.h
    OpticalSystemWrapper/QLensDataEditor.h
    #OpticalSystemWrapper/QFieldSpec.h
    #OpticalSystemWrapper/QOpticalSpec.h
    #OpticalSystemWrapper/QPupilSpec.h

    TextViewDock.h
    PlotViewDock.h

    renderer_qcp.h

    qdebugstream.h

    GeneralConfigDlg.h

    SystemEditor/SystemEditorDock.h
    SystemEditor/SystemEditorWidget.h
    SystemEditor/SurfacePropertyDlg.h

    SystemEditor/LensSpreadSheet.h
    SystemEditor/SpreadSheetItem.h
    SystemEditor/FloatDelegate.h
    SystemEditor/OneByteDelegate.h
    SystemEditor/FieldTable.h
    SystemEditor/WavelengthTable.h
    SystemEditor/SpecEditorDlg.h

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
    AnalysisDlg/OpdFanDlg.h
    AnalysisDlg/WavefrontMapDlg.h
    AnalysisDlg/FFT_PSFDlg.h
    AnalysisDlg/GeoMtfDlg.h
    AnalysisDlg/FFT_MTFDlg.h

)


set(GEOPTER_GUI_SOURCES
    main.cpp

    MainWindow.cpp

    OpticalSystemWrapper/QOpticalSystem.cpp
    OpticalSystemWrapper/QLensDataEditor.cpp
    #OpticalSystemWrapper/QFieldSpec.cpp
    #OpticalSystemWrapper/QOpticalSpec.cpp
    #OpticalSystemWrapper/QPupilSpec.cpp

    TextViewDock.cpp
    PlotViewDock.cpp

    renderer_qcp.cpp

    GeneralConfigDlg.cpp

    SystemEditor/SystemEditorDock.cpp
    SystemEditor/SystemEditorWidget.cpp
    SystemEditor/SurfacePropertyDlg.cpp
    SystemEditor/LensSpreadSheet.cpp
    SystemEditor/SpreadSheetItem.cpp
    SystemEditor/FloatDelegate.cpp
    SystemEditor/OneByteDelegate.cpp
    SystemEditor/FieldTable.cpp
    SystemEditor/WavelengthTable.cpp
    SystemEditor/SpecEditorDlg.cpp

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
    AnalysisDlg/OpdFanDlg.cpp
    AnalysisDlg/WavefrontMapDlg.cpp
    AnalysisDlg/FFT_PSFDlg.cpp
    AnalysisDlg/GeoMtfDlg.cpp
    AnalysisDlg/FFT_MTFDlg.cpp

)

set(GEOPTER_GUI_FORMS
    MainWindow.ui

    GeneralConfigDlg.ui
    SystemEditor/SurfacePropertyDlg.ui
    SystemEditor/SpecEditorDlg.ui

    AnalysisDlg/PrescriptionDlg.ui
    AnalysisDlg/Layout2dDlg.ui
    AnalysisDlg/SingleRayTraceDlg.ui
    AnalysisDlg/SpotDiagramDlg.ui
    AnalysisDlg/ChromaticFocusShiftDlg.ui
    AnalysisDlg/LongitudinalDlg.ui
    AnalysisDlg/ParaxialTraceDlg.ui
    AnalysisDlg/TransverseRayFanDlg.ui
    AnalysisDlg/FieldCurvatureDlg.ui
    AnalysisDlg/OpdFanDlg.ui
    AnalysisDlg/WavefrontMapDlg.ui
    AnalysisDlg/FFT_PSFDlg.ui
    AnalysisDlg/GeoMtfDlg.ui
    AnalysisDlg/FFT_MTFDlg.ui
)

add_executable(${PROJECT_NAME} ${GEOPTER_GUI_SOURCES} ${GEOPTER_GUI_HEADERS} ${GEOPTER_GUI_FORMS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/3rdparty/Qt-Advanced-Docking
    ${CMAKE_SOURCE_DIR}/3rdparty/QCustomPlot
    ${CMAKE_SOURCE_DIR}/3rdparty/pythonqt/src
    ${CMAKE_SOURCE_DIR}/3rdparty/pythonqt/extensions/PythonQt_QtAll
    ${CMAKE_SOURCE_DIR}/3rdparty/eigen-3.3.9
    ${CMAKE_SOURCE_DIR}/3rdparty/spline/src
    ${CMAKE_SOURCE_DIR}/3rdparty
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

target_link_libraries(${PROJECT_NAME} PUBLIC
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

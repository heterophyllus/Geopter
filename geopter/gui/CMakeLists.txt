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
message(STATUS "PYTHON_NUMPY_INCLUDE_DIR= ${PYTHON_NUMPY_INCLUDE_DIR}")

set(GEOPTER_GUI_HEADERS
    MainWindow.h

    OpticalSystemWrapper/QOpticalSystem.h
    OpticalSystemWrapper/QLensDataEditor.h
    #OpticalSystemWrapper/QFieldSpec.h
    #OpticalSystemWrapper/QOpticalSpec.h
    #OpticalSystemWrapper/QPupilSpec.h

    Dock/TextViewDock.h
    Dock/PlotViewDock.h
    Dock/AnalysisViewDock.h

    renderer_qcp.h

    qdebugstream.h

    GeneralConfigDlg.h

    SystemEditor/SystemEditorDock.h
    SystemEditor/SystemEditorWidget.h
    SystemEditor/SystemDataConstant.h

    SystemEditor/LensDataEditor/LensDataTableView.h
    SystemEditor/LensDataEditor/LensDataTableModel.h
    SystemEditor/LensDataEditor/TableHeaderItem.h
    SystemEditor/LensDataEditor/GridTableHeaderView.h
    SystemEditor/LensDataEditor/GridTableHeaderModel.h
    SystemEditor/LensDataEditor/SurfacePropertyDlg.h

    SystemEditor/SolveDlg/SolveSelectionDlg.h
    SystemEditor/SolveDlg/EdgeThicknessSolveDlg.h
    SystemEditor/SolveDlg/OverallLengthSolveDlg.h
    SystemEditor/SolveDlg/MarginalHeightSolveDlg.h

    SystemEditor/Delegate/FloatDelegate.h
    SystemEditor/Delegate/OneByteDelegate.h
    SystemEditor/Delegate/NotEditableDelegate.h

    SystemEditor/SpecEditor/SpecEditorDlg.h
    SystemEditor/SpecEditor/FieldTableView.h
    SystemEditor/SpecEditor/FieldTableModel.h
    SystemEditor/SpecEditor/FieldData.h
    SystemEditor/SpecEditor/WavelengthTableView.h
    SystemEditor/SpecEditor/WavelengthTableModel.h
    SystemEditor/SpecEditor/WavelengthData.h

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

    Dock/TextViewDock.cpp
    Dock/PlotViewDock.cpp
    Dock/AnalysisViewDock.cpp

    renderer_qcp.cpp

    GeneralConfigDlg.cpp

    SystemEditor/SystemEditorDock.cpp
    SystemEditor/SystemEditorWidget.cpp

    SystemEditor/LensDataEditor/LensDataTableView.cpp
    SystemEditor/LensDataEditor/LensDataTableModel.cpp
    SystemEditor/LensDataEditor/TableHeaderItem.cpp
    SystemEditor/LensDataEditor/GridTableHeaderView.cpp
    SystemEditor/LensDataEditor/GridTableHeaderModel.cpp
    SystemEditor/LensDataEditor/SurfacePropertyDlg.cpp

    SystemEditor/Delegate/FloatDelegate.cpp
    SystemEditor/Delegate/OneByteDelegate.cpp

    SystemEditor/SpecEditor/SpecEditorDlg.cpp
    SystemEditor/SpecEditor/FieldTableView.cpp
    SystemEditor/SpecEditor/FieldTableModel.cpp
    SystemEditor/SpecEditor/WavelengthTableView.cpp
    SystemEditor/SpecEditor/WavelengthTableModel.cpp
    
    #AnalysisDlg/AnalysisSettingDlg.cpp
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
    SystemEditor/LensDataEditor/SurfacePropertyDlg.ui
    SystemEditor/SpecEditor/SpecEditorDlg.ui

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
    ${CMAKE_SOURCE_DIR}/3rdparty/qcustomplot
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

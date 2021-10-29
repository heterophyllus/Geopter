cmake_minimum_required (VERSION 3.10)

project(PythonQt_QtAll LANGUAGES CXX)


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

set(PYTHONQT_QTALL_SOURCES
    PythonQt_QtAll/PythonQt_QtAll.cpp
    ${GENERATED_CPP_56_BUILIN_SOURCES}
    ${GENERATED_CPP_56_ALL_SOURCES}
)

add_library(${PROJECT_NAME} SHARED
    ${PYTHONQT_QTALL_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${Python_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/3rdparty/pythonqt/src
    ${GENERATED_CPP_56_BUILTIN_INCLUDE_DIR}
    ${GENERATED_CPP_56_ALL_INCLUDE_DIR}
)

target_compile_definitions(PythonQt_QtAll PRIVATE
    PYTHONQT_QTALL_EXPORTS
    PYTHONQT_WITH_CORE
    PYTHONQT_WITH_GUI
    PYTHONQT_WITH_SVG
    PYTHONQT_WITH_OPENGL
    PYTHONQT_WITH_MULTIMEDIA
    PYTHONQT_WITH_UITOOLS
)

target_link_libraries(PythonQt_QtAll PRIVATE
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
    PythonQt
    ${Python_LIBRARIES}
)

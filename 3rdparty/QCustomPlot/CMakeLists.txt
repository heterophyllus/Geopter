cmake_minimum_required(VERSION 3.5)

project(QCustomPlot)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
find_package(Qt5 COMPONENTS Widgets PrintSupport REQUIRED)

add_library(QCustomPlot STATIC
    qcustomplot.cpp
    )
#target_include_directories(QCustomPlot PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(QCustomPlot PUBLIC Qt5::Widgets Qt5::PrintSupport)

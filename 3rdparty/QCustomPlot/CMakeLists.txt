cmake_minimum_required(VERSION 3.5)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets PrintSupport REQUIRED)

add_library(QCustomPlot STATIC
    qcustomplot.cpp
    )

target_link_libraries(QCustomPlot PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport)

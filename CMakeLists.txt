cmake_minimum_required(VERSION 3.5)

# If QtCreator raises MSB8020 error, check build target in project->build->build step:
# cmake --build --target ALL_BUILD (<- sometimes "all" has been set)
project(Geopter LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY= ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")


## ***************************************************************************************
## ** Qt
## ***************************************************************************************
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# If cmake raises "QT_DIR not found" error, set Qt install path explicitly.
# set(CMAKE_PREFIX_PATH "C:/Qt/(version)/(kit)")

find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport Multimedia MultimediaWidgets OpenGL SVG uitools REQUIRED)

add_subdirectory(3rdparty/Qt-Advanced-Docking)
add_subdirectory(3rdparty/QCustomPlot)
add_subdirectory(3rdparty/pythonqt)
add_subdirectory(3rdparty/matplotplusplus)
add_subdirectory(geopter)

file(COPY "${CMAKE_SOURCE_DIR}/data/AGF" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY "${CMAKE_SOURCE_DIR}/example" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

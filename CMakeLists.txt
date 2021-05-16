cmake_minimum_required(VERSION 3.5)

# If QtCreator raises MSB8020 error, check build target in project->build->build step:
# cmake --build --target ALL_BUILD (<- sometimes "all" has been set)
project(Geopter LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
message(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
# In case cmake raises "QT_DIR not found" error, set Qt install path explicitly.
# set(CMAKE_PREFIX_PATH "C:/Qt/(version)/(kit)")

find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport REQUIRED)

include_directories(3rdparty)
include_directories(3rdparty/spline/src)
include_directories(3rdparty/QCustomPlot)
include_directories(3rdparty/Qt-Advanced-Docking)
include_directories(optical/include)

add_subdirectory( 3rdparty/Qt-Advanced-Docking )
add_subdirectory( 3rdparty/QCustomPlot )

add_subdirectory(src)
add_subdirectory(optical)


file(COPY "${CMAKE_SOURCE_DIR}/AGF" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY "${CMAKE_SOURCE_DIR}/example" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

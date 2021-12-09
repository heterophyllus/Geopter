set(TRIPLET_SOURCES triplet.cpp)

add_executable(triplet ${TRIPLET_SOURCES})

target_include_directories(triplet PUBLIC
    ${CMAKE_SOURCE_DIR}/geopter/optical/include
    ${CMAKE_SOURCE_DIR}/3rdparty #nlohman, svg
    ${CMAKE_SOURCE_DIR}/3rdparty/spline/src
    ${CMAKE_SOURCE_DIR}/3rdparty/eigen-3.3.9
    ${CMAKE_SOURCE_DIR}/3rdparty/matplotplusplus/source/matplot

)

target_link_libraries(triplet PUBLIC
    geopter-optical
    matplot
    )

message(STATUS "TIFF_INCLUDE_DIR= ${TIFF_INCLUDE_DIR}")
message(STATUS "TIFF_LIBRARIES= ${TIFF_LIBRARIES}")

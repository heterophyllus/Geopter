set(TRIPLET_SOURCES triplet.cpp)

add_executable(triplet ${TRIPLET_SOURCES})

target_include_directories(triplet PRIVATE
    ${CMAKE_SOURCE_DIR}/geopter/optical/include
    ${CMAKE_SOURCE_DIR}/3rdparty #Eigen, nlohman
    ${CMAKE_SOURCE_DIR}/3rdparty/spline/src
)

target_link_libraries(triplet PRIVATE geopter-optical)


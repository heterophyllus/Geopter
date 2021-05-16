set(TEST_SOURCES sigma40mmf14.cpp)

get_filename_component( FNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_executable(${FNAME} ${TEST_SOURCES})
target_link_libraries(${FNAME} GeopterOptical)

set_target_properties(photolens
    PROPERTIES
    #ARCHIVE_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib/test>
    #LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib/test>
    RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib/test> # in GeopterOptical test
)

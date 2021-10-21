cmake_minimum_required (VERSION 3.10)


set(GENERATED_CPP_56_BUILTIN_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_core_builtin
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui_builtin
)

set(GENERATED_CPP_56_ALL_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_core
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_multimedia
    #generated_cpp_511/com_trolltech_qt_network
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_opengl
    #generated_cpp_511/com_trolltech_qt_sql
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_svg
    #generated_cpp_511/com_trolltech_qt_xml
    #generated_cpp_511/com_trolltech_qt_xmlpatterns
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_uitools
)

set(GENERATED_CPP_56_BUILTIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_core_builtin/com_trolltech_qt_core_builtin0.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_core_builtin/com_trolltech_qt_core_builtin_init.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui_builtin/com_trolltech_qt_gui_builtin0.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui_builtin/com_trolltech_qt_gui_builtin_init.cpp
    )

set(GENERATED_CPP_56_ALL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_core/com_trolltech_qt_core_init.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_core/com_trolltech_qt_core0.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_core/com_trolltech_qt_core1.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_core/com_trolltech_qt_core2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_core/com_trolltech_qt_core3.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui/com_trolltech_qt_gui_init.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui/com_trolltech_qt_gui0.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui/com_trolltech_qt_gui1.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui/com_trolltech_qt_gui2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui/com_trolltech_qt_gui3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui/com_trolltech_qt_gui4.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui/com_trolltech_qt_gui5.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui/com_trolltech_qt_gui6.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui/com_trolltech_qt_gui7.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui/com_trolltech_qt_gui8.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui/com_trolltech_qt_gui9.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui/com_trolltech_qt_gui10.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui/com_trolltech_qt_gui11.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_gui/com_trolltech_qt_gui12.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_multimedia/com_trolltech_qt_multimedia_init.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_multimedia/com_trolltech_qt_multimedia0.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_multimedia/com_trolltech_qt_multimedia1.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_multimedia/com_trolltech_qt_multimedia2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_multimedia/com_trolltech_qt_multimedia3.cpp

#    com_trolltech_qt_network/com_trolltech_qt_network0.cpp
#    com_trolltech_qt_network/com_trolltech_qt_network1.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_opengl/com_trolltech_qt_opengl_init.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_opengl/com_trolltech_qt_opengl0.cpp

#    com_trolltech_qt_sql/com_trolltech_qt_sql0.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_svg/com_trolltech_qt_svg_init.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_svg/com_trolltech_qt_svg0.cpp

#    com_trolltech_qt_xml/com_trolltech_qt_xml0.cpp
#    com_trolltech_qt_xml/com_trolltech_qt_xml1.cpp

#    com_trolltech_qt_xmlpatterns/com_trolltech_qt_xmlpatterns0.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_uitools/com_trolltech_qt_uitools_init.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_cpp_56/com_trolltech_qt_uitools/com_trolltech_qt_uitools0.cpp
    )


#add_subdirectory(generated_cpp_56)
add_subdirectory(src)
add_subdirectory(extensions)




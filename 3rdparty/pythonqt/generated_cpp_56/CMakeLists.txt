cmake_minimum_required (VERSION 3.10)


set(MODULE_GEN_CPP_SOURCES
    com_trolltech_qt_core_builtin/com_trolltech_qt_core_builtin0.cpp
    com_trolltech_qt_core_builtin/com_trolltech_qt_core_builtin_init.cpp

    com_trolltech_qt_gui_builtin/com_trolltech_qt_gui_builtin0.cpp
    com_trolltech_qt_gui_builtin/com_trolltech_qt_gui_builtin_init.cpp
    )

set(MODULE_GEN_CPP_SOURCES_WITH_DIR "")
foreach (src_file ${MODULE_GEN_CPP_SOURCES})
    list(APPEND MODULE_GEN_CPP_SOURCES_WITH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${src_file})
endforeach (src_file)

list(APPEND GENERATED_CPP_SOURCES ${MODULE_GEN_CPP_SOURCES_WITH_DIR})
set(GENERATED_CPP_SOURCES "${GENERATED_CPP_SOURCES}" PARENT_SCOPE)


set(MODULE_GEN_CPP_ALL_SOURCES
    com_trolltech_qt_core/com_trolltech_qt_core_init.cpp
    com_trolltech_qt_core/com_trolltech_qt_core0.cpp
    com_trolltech_qt_core/com_trolltech_qt_core1.cpp
    com_trolltech_qt_core/com_trolltech_qt_core2.cpp
    com_trolltech_qt_core/com_trolltech_qt_core3.cpp

    com_trolltech_qt_gui/com_trolltech_qt_gui_init.cpp
    com_trolltech_qt_gui/com_trolltech_qt_gui0.cpp
    com_trolltech_qt_gui/com_trolltech_qt_gui1.cpp
    com_trolltech_qt_gui/com_trolltech_qt_gui2.cpp
    com_trolltech_qt_gui/com_trolltech_qt_gui3.cpp
    com_trolltech_qt_gui/com_trolltech_qt_gui4.cpp
    com_trolltech_qt_gui/com_trolltech_qt_gui5.cpp
    com_trolltech_qt_gui/com_trolltech_qt_gui6.cpp
    com_trolltech_qt_gui/com_trolltech_qt_gui7.cpp
    com_trolltech_qt_gui/com_trolltech_qt_gui8.cpp
    com_trolltech_qt_gui/com_trolltech_qt_gui9.cpp
    com_trolltech_qt_gui/com_trolltech_qt_gui10.cpp
    com_trolltech_qt_gui/com_trolltech_qt_gui11.cpp
    com_trolltech_qt_gui/com_trolltech_qt_gui12.cpp

    com_trolltech_qt_multimedia/com_trolltech_qt_multimedia_init.cpp
    com_trolltech_qt_multimedia/com_trolltech_qt_multimedia0.cpp
    com_trolltech_qt_multimedia/com_trolltech_qt_multimedia1.cpp
    com_trolltech_qt_multimedia/com_trolltech_qt_multimedia2.cpp
    com_trolltech_qt_multimedia/com_trolltech_qt_multimedia3.cpp

#    com_trolltech_qt_network/com_trolltech_qt_network0.cpp
#    com_trolltech_qt_network/com_trolltech_qt_network1.cpp

    com_trolltech_qt_opengl/com_trolltech_qt_opengl_init.cpp
    com_trolltech_qt_opengl/com_trolltech_qt_opengl0.cpp

#    com_trolltech_qt_sql/com_trolltech_qt_sql0.cpp

    com_trolltech_qt_svg/com_trolltech_qt_svg_init.cpp
    com_trolltech_qt_svg/com_trolltech_qt_svg0.cpp

#    com_trolltech_qt_xml/com_trolltech_qt_xml0.cpp
#    com_trolltech_qt_xml/com_trolltech_qt_xml1.cpp

#    com_trolltech_qt_xmlpatterns/com_trolltech_qt_xmlpatterns0.cpp

    com_trolltech_qt_uitools/com_trolltech_qt_uitools_init.cpp
    com_trolltech_qt_uitools/com_trolltech_qt_uitools0.cpp
    )

set(MODULE_GEN_CPP_ALL_SOURCES_WITH_DIR "")
foreach (src_file ${MODULE_GEN_CPP_ALL_SOURCES})
    list(APPEND MODULE_GEN_CPP_ALL_SOURCES_WITH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${src_file})
endforeach (src_file)


list(APPEND GENERATED_CPP_ALL_SOURCES ${MODULE_GEN_CPP_ALL_SOURCES_WITH_DIR})
set(GENERATED_CPP_ALL_SOURCES "${GENERATED_CPP_ALL_SOURCES}" PARENT_SCOPE)


project(PyGuiExample LANGUAGES CXX)

add_executable(PyGuiExample
    main.cpp
    PyGuiExample.qrc
)

target_include_directories(PyGuiExample PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extensions/PythonQt_QtAll
    ${Python_INCLUDE_DIRS}
)

target_compile_definitions(PyGuiExample PRIVATE
    PYTHONQT_EXPORTS
    PYTHONQT_QTALL_EXPORTS
    PYTHONQT_WITH_CORE
    PYTHONQT_WITH_GUI
    PYTHONQT_WITH_SVG
    PYTHONQT_WITH_OPENGL
    PYTHONQT_WITH_MULTIMEDIA
    PYTHONQT_WITH_UITOOLS
)

target_link_libraries(PyGuiExample PRIVATE
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
    PythonQt_QtAll
    ${Python_LIBRARIES}
)

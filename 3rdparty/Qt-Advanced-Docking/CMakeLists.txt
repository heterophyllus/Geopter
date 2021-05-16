cmake_minimum_required(VERSION 3.5)
project(QtAdvancedDockingSystem LANGUAGES CXX VERSION ${VERSION_SHORT})

#find_package(Qt5 5.15 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

if (UNIX AND NOT APPLE)
	find_package(Qt5 5.5 COMPONENTS X11Extras REQUIRED)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(BUILD_STATIC)
    set(CMAKE_STATIC_LIBRARY_SUFFIX "_static${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()

set(ads_SRCS
    ads_globals.cpp
    DockAreaTabBar.cpp
    DockAreaTitleBar.cpp
    DockAreaWidget.cpp
    DockContainerWidget.cpp
    DockManager.cpp
    DockOverlay.cpp
    DockSplitter.cpp
    DockWidget.cpp
    DockWidgetTab.cpp
    DockingStateReader.cpp
    DockFocusController.cpp
    ElidingLabel.cpp
    FloatingDockContainer.cpp
    FloatingDragPreview.cpp
    IconProvider.cpp
    DockComponentsFactory.cpp
    ads.qrc
)
set(ads_HEADERS
    ads_globals.h
    DockAreaTabBar.h
    DockAreaTitleBar.h
    DockAreaTitleBar_p.h
    DockAreaWidget.h
    DockContainerWidget.h
    DockManager.h
    DockOverlay.h
    DockSplitter.h
    DockWidget.h
    DockWidgetTab.h
    DockingStateReader.h
    DockFocusController.h
    ElidingLabel.h
    FloatingDockContainer.h
    FloatingDragPreview.h
    IconProvider.h
    DockComponentsFactory.h
)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
if (UNIX AND NOT APPLE)
    set(ads_SRCS linux/FloatingWidgetTitleBar.cpp ${ads_SRCS})
    set(ads_HEADERS linux/FloatingWidgetTitleBar.h ${ads_HEADERS})
endif()
if(BUILD_STATIC)
    add_library(qtadvanceddocking STATIC ${ads_SRCS} ${ads_HEADERS})
    target_compile_definitions(qtadvanceddocking PUBLIC ADS_STATIC)
else()
    add_library(qtadvanceddocking SHARED ${ads_SRCS} ${ads_HEADERS})
    target_compile_definitions(qtadvanceddocking PRIVATE ADS_SHARED_EXPORT)
endif()
target_link_libraries(qtadvanceddocking PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)
if(UNIX AND NOT APPLE)
    target_link_libraries(qtadvanceddocking PUBLIC Qt5::X11Extras)
	target_link_libraries(qtadvanceddocking PRIVATE xcb)
endif()

set_target_properties(qtadvanceddocking
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

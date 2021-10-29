cmake_minimum_required (VERSION 3.10)

project(PythonQt LANGUAGES CXX)


## ***************************************************************************************
## ** Qt
## ***************************************************************************************
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# If cmake raises "QT_DIR not found" error, set Qt install path explicitly.
# set(CMAKE_PREFIX_PATH "C:/Qt/(version)/(kit)")

find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport Multimedia MultimediaWidgets OpenGL SVG uitools REQUIRED)


## ***************************************************************************************
## ** Python
## ***************************************************************************************
#set(Python_ADDITIONAL_VERSIONS 3.9)
find_package(Python COMPONENTS Interpreter Development REQUIRED)
message(STATUS "Python_FOUND= ${Python_FOUND}")
message(STATUS "Python_VERSION= ${Python_VERSION}")
message(STATUS "Python_INCLUDE_DIRS= ${Python_INCLUDE_DIRS}")
message(STATUS "Python_LIBRARIES= ${Python_LIBRARIES}")
message(STATUS "Python_NumPy_FOUND= ${Python_NumPy_FOUND}")

set(PYTHONQT_SOURCES
  ${GENERATED_CPP_56_BUILTIN_SOURCES}
  PythonQtStdDecorators.cpp 
  PythonQt.cpp              
  PythonQtClassInfo.cpp      
  PythonQtImporter.cpp       
  PythonQtObjectPtr.cpp      
  PythonQtProperty.cpp       
  PythonQtStdIn.cpp          
  PythonQtStdOut.cpp         
  PythonQtSignal.cpp        
  PythonQtSlot.cpp           
  PythonQtSlotDecorator.cpp  
  PythonQtMisc.cpp          
  PythonQtMethodInfo.cpp     
  PythonQtConversion.cpp     
  PythonQtSignalReceiver.cpp 
  PythonQtInstanceWrapper.cpp 
  PythonQtQFileImporter.cpp  
  PythonQtClassWrapper.cpp  
  PythonQtBoolResult.cpp     
  gui/PythonQtScriptingConsole.cpp
  PythonQtThreadSupport.cpp 

)

add_library(PythonQt SHARED ${PYTHONQT_SOURCES} )

target_include_directories(PythonQt PRIVATE
    ${Qt5Core_PRIVATE_INCLUDE_DIRS}
    ${Python_INCLUDE_DIRS}
    ${GENERATED_CPP_56_BUILTIN_INCLUDE_DIR}
)

target_compile_definitions(PythonQt PRIVATE
    PYTHONQT_EXPORTS
    PYTHONQT_CATCH_ALL_EXCEPTIONS
)

target_link_libraries(PythonQt PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    ${Python_LIBRARIES}
)

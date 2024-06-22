cmake_minimum_required(VERSION 2.8.3)
project(cira_bernoulli_opencv)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)

find_package(catkin REQUIRED
    roscpp
    cira_lib_bernoulli cira_std_lib
    )

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS
        roscpp
        cira_lib_bernoulli cira_std_lib
)

include_directories(
 include
 models
 ${catkin_INCLUDE_DIRS}
)

find_package(NodeEditor REQUIRED)

if(UNIX AND NOT APPLE)
        set(LINUX TRUE)
endif()

if(LINUX)
    add_compile_options(-Wall -Wextra)

    if(NOT RASP)
        find_package(OpenCV 4 REQUIRED
        NO_MODULE
        PATHS /usr/local
        NO_DEFAULT_PATH)
    else()
        find_package(OpenCV 4 REQUIRED)
    endif()
    include_directories(${OpenCV_INCLUDE_DIRS})

    if(RASP)
        add_definitions(-DRASP)
    endif()

    if(TERMUX)
        add_definitions(-DTERMUX)
        find_package(OpenCV 4 REQUIRED
            NO_MODULE
            PATHS /data/data/com.termux/files/home/cira_depend/opencv-4.6.0/install
            NO_DEFAULT_PATH)
        include_directories(${OpenCV_INCLUDE_DIRS})
    elseif(RASP)
        find_package(OpenCV 4 REQUIRED)
        include_directories(${OpenCV_INCLUDE_DIRS})
    endif()

elseif(APPLE)
    find_package(OpenCV REQUIRED)
    include_directories(${OpenCV_INCLUDE_DIRS})

else()

    add_definitions("/std:c++14")

    find_package(OpenCV 4 REQUIRED)
    include_directories(${OpenCV_INCLUDE_DIRS})
    message(STATUS "*******************************************")
    message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
    link_directories( /CiRA-CORE/install/x64/vc16/lib )
    set(OpenCV_LIBRARIES
        opencv_core460 opencv_videoio460
        opencv_highgui460 opencv_imgproc460
        opencv_imgcodecs460
    )
    message(STATUS "##################################OpenCV_LIBRARIES: ${OpenCV_LIBRARIES}")

endif()

set(qt_version qt511)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /opt/${qt_version})
set(Qt5Core_DIR /opt/${qt_version}/lib/cmake/Qt5Core)
set(Qt5Widgets_DIR /opt/${qt_version}/lib/cmake/Qt5Widgets)
set(Qt5Gui_DIR /opt/${qt_version}/lib/cmake/Qt5Gui)
set(Qt5_DIR /opt/${qt_version}/lib/cmake/Qt5)
set(QT5_LIBRARIES /opt/${qt_version}/lib)

find_package(Qt5 COMPONENTS
             Core
             Widgets
             Gui
             )

file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ui/*.ui
    models/*.ui
    )
file(GLOB QT_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    resources/*.qrc
    )
file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS
    include/*.hpp
    include/*.h

    models/*.hpp
    models/*.h
    )

QT5_ADD_RESOURCES(QT_RESOURCES_CPP ${QT_RESOURCES})
QT5_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT5_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

file(GLOB_RECURSE QT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS
    include/*.cpp
    src/*.cpp
    models/*.cpp
    )

add_library(${PROJECT_NAME}
    ${QT_SOURCES}
    ${QT_RESOURCES_CPP}
    ${QT_FORMS_HPP}
    ${QT_MOC_HPP}
    )

if(MSVC)
  target_link_libraries(${PROJECT_NAME}
    	${QT_LIBRARIES} ${catkin_LIBRARIES}
    	${OpenCV_LIBRARIES}
    	Qt5::Core Qt5::Widgets Qt5::Gui
    	NodeEditor::nodes
  )
  
  install(TARGETS ${PROJECT_NAME}
  	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
  install(FILES ${CATKIN_DEVEL_PREFIX}/bin/${PROJECT_NAME}.dll
  	DESTINATION $ENV{HOMEPATH}/CiRA_CORE_Plugins)
else()
  target_link_libraries(${PROJECT_NAME}
        ${QT_LIBRARIES} ${catkin_LIBRARIES}
        ${OpenCV_LIBRARIES}
        Qt5::Core Qt5::Widgets Qt5::Gui
  )

  install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION $ENV{HOME}/CiRA_CORE_Plugins)
endif()

###################

#add_executable(${PROJECT_NAME}_test test_main.cpp)
#target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})

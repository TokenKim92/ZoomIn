cmake_minimum_required(VERSION 3.5)

project(ZoomIn VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH C:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5)

find_package(Qt5 COMPONENTS core gui widgets REQUIRED)

if(WIN32)
    add_executable(ZoomIn WIN32
        main.cpp
        mainwindow.cpp
        mainwindow.h
        icon.rc
    )
else()
    add_executable(ZoomIn
        main.cpp
        mainwindow.cpp
        mainwindow.h
    )
endif()

target_link_libraries(ZoomIn PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

set_target_properties(ZoomIn PROPERTIES
      CXX_STANDARD 11
      CXX_STANDARD_REQUIRED ON
)

install(TARGETS ZoomIn
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin
)

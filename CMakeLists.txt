cmake_minimum_required(VERSION 3.5)

set(TARGET_NAME ZoomIn)

project(${TARGET_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH C:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5)
set(TARGET_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(TARGET_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

find_package(Qt5 COMPONENTS core gui widgets REQUIRED)

if(WIN32)
    add_executable(${TARGET_NAME} WIN32
        ${TARGET_SRC_DIR}/main.cpp
        ${TARGET_SRC_DIR}/mainwindow.cpp
        ${TARGET_INCLUDE_DIR}/mainwindow.h
        ${CMAKE_SOURCE_DIR}/icon.rc
    )
else()
    add_executable(${TARGET_NAME}
        ${TARGET_SRC_DIR}/main.cpp
        ${TARGET_SRC_DIR}/mainwindow.cpp
        ${TARGET_INCLUDE_DIR}/mainwindow.h
    )
endif()

target_link_libraries(${TARGET_NAME} 
PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

target_include_directories(${TARGET_NAME}
PRIVATE
    ${TARGET_SRC_DIR}
PUBLIC
    ${TARGET_INCLUDE_DIR}
)

set_target_properties(${TARGET_NAME} 
PROPERTIES
      CXX_STANDARD 11
      CXX_STANDARD_REQUIRED ON
)

install(TARGETS ${TARGET_NAME}
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin
)
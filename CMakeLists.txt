cmake_minimum_required(VERSION 3.14)
project(ftxui-image-view)
include(cmake/ftxui.cmake)

set(CMAKE_CXX_STANDARD 17)

add_library(tiv
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/tiv_lib.cpp
)

target_link_libraries(tiv
    PRIVATE X11
)

add_library(ftxui-image-view STATIC
    include/image_view.hpp
    src/image_view.cpp
)

target_include_directories(ftxui-image-view PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(ftxui-image-view PUBLIC ftxui::component ftxui::dom ftxui::screen tiv)
add_subdirectory(example)

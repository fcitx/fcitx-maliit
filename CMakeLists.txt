project(fcitx-maliit)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)
find_package(PkgConfig REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

pkg_check_modules(MALIIT_PLUGINS REQUIRED "maliit-plugins-0.80" )

set(prefix ${CMAKE_INSTALL_PREFIX})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_subdirectory(src)
add_subdirectory(qml)
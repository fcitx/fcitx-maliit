project(fcitx-maliit)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)
find_package(PkgConfig REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

pkg_check_modules(MALIIT_PLUGINS REQUIRED "maliit-plugins-0.80" )
_pkgconfig_invoke("maliit-plugins-0.80" MALIIT_PLUGINS INSTALL_DIR "" "--variable=pluginsdir")

message("maliit-plguins install dir: ${MALIIT_PLUGINS_INSTALL_DIR}")

find_package(Fcitx 4.1.3 REQUIRED)

set(prefix ${CMAKE_INSTALL_PREFIX})

set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden ${CMAKE_CXX_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--as-needed,--no-undefined ${CMAKE_MODULE_LINKER_FLAGS}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_subdirectory(src)
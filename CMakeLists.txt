cmake_minimum_required(VERSION 2.6)

project(CommonLib)

file(GLOB_RECURSE C_FILES CommonLib/*.c)
file(GLOB_RECURSE H_FILES CommonLib/*.h)
include_directories(.)
add_library(CommonLib ${C_FILES} ${H_FILES})

find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
   pkg_check_modules(SYSTEMD "systemd" REQUIRED)
   include_directories(SYSTEMD_INCLUDE_DIR)
endif (PKG_CONFIG_FOUND)

add_executable(echo_server Examples/echo_server.c)
target_link_libraries(echo_server CommonLib systemd)

add_executable(echo_client Examples/echo_client.c)
target_link_libraries(echo_client CommonLib)

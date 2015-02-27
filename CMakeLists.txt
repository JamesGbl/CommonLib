cmake_minimum_required(VERSION 2.6)

project(CommonLib)

find_package(PostgreSQL)

file(GLOB_RECURSE C_FILES CommonLib/*.c)
file(GLOB_RECURSE H_FILES CommonLib/*.h)
include_directories(.)
include_directories(${PostgreSQL_INCLUDE_DIR})
add_library(CommonLib ${C_FILES} ${H_FILES})
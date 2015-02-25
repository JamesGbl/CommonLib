cmake_minimum_required(VERSION 2.6)

project(CommonLib)

file(GLOB_RECURSE C_FILES CommonLib/*.c)
file(GLOB_RECURSE H_FILES CommonLib/*.h)
include_directories(.)
add_library(CommonLib ${C_FILES} ${H_FILES})
cmake_minimum_required(VERSION 2.8)

project(argv)

file(GLOB_RECURSE ARGV_SOURCE_FILES "src/*.c")

set (CMAKE_C_FLAGS "-g -Wall")

include_directories(${PROJECT_BINARY_DIR}/include)

add_library("argv" ${ARGV_SOURCE_FILES})


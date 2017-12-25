cmake_minimum_required(VERSION 2.6)

project(assertc C)
# version numbers
set(assertc_VERSION_MAJOR 0)
set(assertc_VERSION_MINOR 1)

file(GLOB_RECURSE main_SOURCES "src/*.c")
file(GLOB_RECURSE reporter_SOURCES "src/reporters/*.c")

add_library(assertc ${main_SOURCES} ${reporter_SOURCES})
# add include directories
target_include_directories(assertc
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/reporters
    )



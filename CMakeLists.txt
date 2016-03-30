cmake_minimum_required(VERSION 2.6)

project(assertc)
# version numbers
set(assertc_VERSION_MAJOR 0)
set(assertc_VERSION_MINOR 1)

# add include directories
include_directories("${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/src/reporters")
file(GLOB_RECURSE main_SOURCES "src/*.c")
file(GLOB_RECURSE reporter_SOURCES "src/reporters/*.c")

add_library(assertc ${main_SOURCES} ${reporter_SOURCES})


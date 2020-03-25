cmake_minimum_required(VERSION 3.17)

project(volume_augmentations)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

enable_language(Rust)
# https://github.com/Devolutions/CMakeRust
include(CMakeCargo)

add_subdirectory(vacpp)
add_subdirectory(vars)

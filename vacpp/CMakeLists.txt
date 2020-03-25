cmake_minimum_required(VERSION 3.17)

project(volume_augmentations_cpp CXX)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_library(volume_augmentations_cpp STATIC src/foo.cpp)

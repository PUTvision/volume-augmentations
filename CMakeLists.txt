cmake_minimum_required(VERSION 3.12)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()

project(volume_augmentations)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (default Release)" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

enable_language(Rust)
# https://github.com/Devolutions/CMakeRust
include(CMakeCargo)

if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
else()
  message(STATUS "Using custom runtime output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()
if(NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
else()
  message(STATUS "Using custom library output directory: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
endif()
if(NOT DEFINED CMAKE_WHEEL_OUTPUT_DIRECTORY)
  set(CMAKE_WHEEL_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/wheel)
else()
  message(STATUS "Using custom wheel output directory: ${CMAKE_WHEEL_OUTPUT_DIRECTORY}")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_COLOR_MAKEFILE ON)


find_package(Python3 COMPONENTS Interpreter)

add_custom_target(debug
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build .
  COMMENT "Switch CMAKE_BUILD_TYPE to Debug"
  )

add_custom_target(release
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build .
  COMMENT "Switch CMAKE_BUILD_TYPE to Release"
  )
add_custom_target(wheel
  # DEPENDS va_cpp va_rs # TODO(PW) it would be nice if `setup.py` could use the existing build
  COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/setup.py bdist_wheel --dist-dir ${CMAKE_WHEEL_OUTPUT_DIRECTORY}
  BYPRODUCTS ${CMAKE_WHEEL_OUTPUT_DIRECTORY}/*.whl
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Build wheel"
  )
add_custom_target(wheel_install
  DEPENDS wheel
  COMMAND ${Python3_EXECUTABLE} -m pip install ${CMAKE_WHEEL_OUTPUT_DIRECTORY}/*.whl --force-reinstall
  COMMENT "Install wheel"
  )


add_subdirectory(va_cpp)
add_subdirectory(va_rs)

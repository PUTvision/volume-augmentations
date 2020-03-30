cmake_minimum_required(VERSION 3.12)

project(va_cpp CXX)

find_package(Python3 COMPONENTS Development)
find_package(pybind11 CONFIG REQUIRED)
find_package(doctest REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)

Python3_add_library(va_cpp SHARED)
target_link_libraries(va_cpp PRIVATE
  pybind11::embed
  pybind11::module
  pybind11::pybind11
  doctest::doctest
  Eigen3::Eigen)

add_executable(va_cpp_tests)
target_link_libraries(va_cpp_tests va_cpp doctest::doctest)

set_target_properties(va_cpp va_cpp_tests PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  )

set_target_properties(va_cpp PROPERTIES
  PREFIX ""
  SUFFIX ".${Python3_SOABI}${CMAKE_SHARED_MODULE_SUFFIX}"
  )

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)
if (ipo_supported)
  message(STATUS "IPO / LTO enabled")
  set_target_properties(va_cpp va_cpp_tests PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
    INTERPROCEDURAL_OPTIMIZATION_DEBUG FALSE
    )
else()
  message(STATUS "IPO / LTO not supported: <${error}>")
endif()


if (MSVC)
  # warning level 4 and all warnings as errors
  add_compile_options(/W4 /WX)
else()
  # lots of warnings and all warnings as errors
  add_compile_options(
    -Wall
    -Wextra
    -pedantic
    -Werror
    -march=native
    -Weffc++
    -fconcepts
    -fdiagnostics-color=always
    )
endif()

target_sources(va_cpp PRIVATE
  src/transformation.cpp
  src/volume_transformer.cpp
  )

target_sources(va_cpp_tests PRIVATE
  tests/main.cpp
  )

add_test(
  NAME va_cpp_tests
  COMMAND $<TARGET_FILE:va_cpp_tests>
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )

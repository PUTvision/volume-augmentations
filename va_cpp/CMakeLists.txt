cmake_minimum_required(VERSION 3.17)

project(va_cpp CXX)

add_library(va_cpp SHARED)

set_target_properties(va_cpp PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  PREFIX ""
  )

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)
if (ipo_supported)
  message(STATUS "IPO / LTO enabled")
  set_target_properties(va_cpp PROPERTIES
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


find_package(pybind11 CONFIG REQUIRED)
target_link_libraries(va_cpp PRIVATE pybind11::embed pybind11::module pybind11::pybind11)


target_sources(va_cpp PRIVATE
  src/transformation.cpp
  )

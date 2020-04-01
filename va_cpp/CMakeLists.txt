cmake_minimum_required(VERSION 3.12)

project(va_cpp CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(Python3 COMPONENTS Development)
find_package(pybind11 CONFIG REQUIRED)
find_package(doctest REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)

add_library(va_cpp_static OBJECT)
pybind11_add_module(va_cpp SHARED $<TARGET_OBJECTS:va_cpp_static>)
add_executable(va_cpp_tests $<TARGET_OBJECTS:va_cpp_static>)

target_link_libraries(va_cpp_static PUBLIC
  pybind11::embed
  pybind11::module
  pybind11::pybind11
  doctest::doctest
  Eigen3::Eigen)

target_link_libraries(va_cpp_tests PRIVATE
  doctest::doctest)

if (MSVC)
  target_link_libraries(va_cpp_tests PRIVATE
    pybind11::embed
    pybind11::module
    pybind11::pybind11
    Eigen3::Eigen)
endif()

set_target_properties(va_cpp va_cpp_tests va_cpp_static
  PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  )
set_target_properties(va_cpp_static
  PROPERTIES
  POSITION_INDEPENDENT_CODE ON
  )

if (DEFINED Python3_SOABI)
  set_target_properties(va_cpp PROPERTIES
    PREFIX ""
    SUFFIX ".${Python3_SOABI}${CMAKE_SHARED_MODULE_SUFFIX}"
    )
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)
if (ipo_supported)
  message(STATUS "IPO / LTO enabled")
  set_target_properties(va_cpp
    va_cpp_static
    va_cpp_tests
    PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
    INTERPROCEDURAL_OPTIMIZATION_DEBUG FALSE
    )
else()
  message(STATUS "IPO / LTO not supported: <${error}>")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
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

target_sources(va_cpp_static PRIVATE
  src/transformation.cpp
  src/volume_transformer.cpp
  src/utils.cpp
  src/test_entry.cpp
  )
target_sources(va_cpp_static PRIVATE
  src/utils.h
  )


target_sources(va_cpp_tests PRIVATE
  tests/main.cpp
  )

enable_testing()
add_test(
  NAME va_cpp_tests
  COMMAND $<TARGET_FILE:va_cpp_tests>
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )

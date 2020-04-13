cmake_minimum_required(VERSION 3.13)
include(CheckCXXCompilerFlag)

project(va_cpp CXX)

option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
if (${FORCE_COLORED_OUTPUT})
  check_cxx_compiler_flag(-fdiagnostics-color=always COMPILER_COLOR_OUTPUT)
  if (COMPILER_COLOR_OUTPUT)
    add_compile_options (-fdiagnostics-color=always)
  endif()
endif ()

check_cxx_compiler_flag(-fconcepts COMPILER_CONCEPTS_SUPPORT)
if (COMPILER_CONCEPTS_SUPPORT)
  # add_compile_options (-fconcepts)
endif()

set(COMPILE_OPTIONS_MSVC
  )
set(COMPILE_OPTIONS_CLANG
  -fsized-deallocation
  )
set(COMPILE_OPTIONS_GCC
  )
add_compile_options(
  "$<$<CXX_COMPILER_ID:Clang>:${COMPILE_OPTIONS_CLANG}>"
  "$<$<CXX_COMPILER_ID:MSVC>:${COMPILE_OPTIONS_MSVC}>"
  "$<$<CXX_COMPILER_ID:GNU>:${COMPILE_OPTIONS_GCC}>"
  )

message(STATUS "MKLROOT" $ENV{MKLROOT})
set(BLA_VENDOR Intel10_64lp)
set(BLA_STATIC OFF)
find_package(LAPACK REQUIRED)
find_package(BLAS REQUIRED)
# message(STATUS "BLAS VENDOR:    " ${BLA_VENDOR})
# message(STATUS "BLAS LIBRARIES: " ${BLAS_LIBRARIES})
# message(STATUS "BLAS LINKING FLAGS: " ${BLAS_LINKER_FLAGS})
# message(STATUS "LAPACK LIBRARIES: " ${LAPACK_LIBRARIES})
# message(STATUS "LAPACK LINKING FLAGS: " ${LAPACK_LINKER_FLAGS})

find_package(Python3 COMPONENTS Development)
find_package(pybind11 CONFIG REQUIRED)
find_package(doctest REQUIRED)
find_package(xtensor CONFIG REQUIRED)
find_package(xtensor-blas CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)

add_library(va_cpp_static OBJECT)
pybind11_add_module(va_cpp SHARED $<TARGET_OBJECTS:va_cpp_static>)
add_executable(va_cpp_tests $<TARGET_OBJECTS:va_cpp_static>)

target_compile_definitions(va_cpp_static PRIVATE $<$<CONFIG:Debug>:CXXBLAS_DEBUG=1>)

target_compile_definitions(va_cpp_static
  PRIVATE
  -DXTENSOR_ENABLE_XSIMD
  -DXTENSOR_USE_XSIMD
  -DXTENSOR_USE_TBB
  -DHAVE_CBLAS=1
  -DUSE_CXXLAPACK=1
  -DWITH_MKLBLAS=1
  -DTBB_SUPPRESS_DEPRECATED_MESSAGES
  )

set(COMPILE_OPTIONS_WARN_MSVC
  /W4
  /WX
  )
set(COMPILE_OPTIONS_WARN_CLANG
  -Wall
  -Wextra
  -pedantic
  -Werror
  -Weffc++
  -Wno-c99-extensions
  )
set(COMPILE_OPTIONS_WARN_GCC
  -Wall
  -Wextra
  -pedantic
  -Werror
  -Weffc++
  )
target_compile_options(va_cpp PRIVATE
  "$<$<CXX_COMPILER_ID:Clang>:${COMPILE_OPTIONS_CLANG}>"
  "$<$<CXX_COMPILER_ID:MSVC>:${COMPILE_OPTIONS_MSVC}>"
  "$<$<CXX_COMPILER_ID:GNU>:${COMPILE_OPTIONS_GCC}>"
  )
target_compile_options(va_cpp_tests PRIVATE
  "$<$<CXX_COMPILER_ID:Clang>:${COMPILE_OPTIONS_CLANG}>"
  "$<$<CXX_COMPILER_ID:MSVC>:${COMPILE_OPTIONS_MSVC}>"
  "$<$<CXX_COMPILER_ID:GNU>:${COMPILE_OPTIONS_GCC}>"
  )
target_compile_options(va_cpp_static PRIVATE
  "$<$<CXX_COMPILER_ID:Clang>:${COMPILE_OPTIONS_CLANG}>"
  "$<$<CXX_COMPILER_ID:MSVC>:${COMPILE_OPTIONS_MSVC}>"
  "$<$<CXX_COMPILER_ID:GNU>:${COMPILE_OPTIONS_GCC}>"
  )

target_link_libraries(va_cpp_static PUBLIC
  ${BLAS_LIBRARIES}
  ${LAPACK_LIBRARIES}
  TBB::tbb
  xtensor::use_xsimd
  xtensor-blas
  xtensor::optimize
  xtensor::use_TBB
  pybind11::embed
  pybind11::module
  pybind11::pybind11
  doctest::doctest
  )

target_link_libraries(va_cpp PRIVATE
  va_cpp_static
  )
target_link_libraries(va_cpp_tests PRIVATE
  va_cpp_static
  )

if (MSVC)
  # NOTE(przemkovv): By default all dynamic libraries on Windows land in
  # ${CMAKE_BINARY_OUTPUT_DIRECTORY}, thus  we need to copy it to a desired
  # location
  add_custom_command(TARGET va_cpp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:va_cpp> ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    COMMENT "Copying libraries to lib folder")
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

target_sources(va_cpp_static PRIVATE
  src/test_entry.cpp
  src/transform_matrix.cpp
  src/transformation.cpp
  src/utils.cpp
  src/volume_transformer.cpp
  )
target_sources(va_cpp_static PRIVATE
  src/transform_matrix.h
  src/utils.h
  src/volume_transformer.h
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

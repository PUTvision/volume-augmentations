cmake_minimum_required(VERSION 3.12)

project(va_cpp CXX)
set(CMAKE_CXX_STANDARD 20)
add_definitions(-DXTENSOR_ENABLE_XSIMD)
add_definitions(-DXTENSOR_USE_XSIMD)
add_definitions(-DXTENSOR_USE_TBB)
add_definitions(-DHAVE_CBLAS=1)
# add_definitions(-DUSE_OPENBLAS=1)
add_definitions(-DUSE_CXXLAPACK=0)
add_definitions(-DWITH_MKLBLAS=1)
add_definitions(-DCXXBLAS_DEBUG=1)
add_definitions(-DTBB_SUPPRESS_DEPRECATED_MESSAGES)
set(BLA_VENDOR Intel10_64lp)
set(BLA_STATIC OFF)
find_package(LAPACK REQUIRED)
find_package(BLAS REQUIRED)
# message(STATUS "BLAS VENDOR:    " ${BLA_VENDOR})
# message(STATUS "BLAS LIBRARIES: " ${BLAS_LIBRARIES})
# message(STATUS "BLAS LINKING FLAGS: " ${BLAS_LINKER_FLAGS})
# message(STATUS "LAPACK LIBRARIES: " ${LAPACK_LIBRARIES})
# message(STATUS "LAPACK LINKING FLAGS: " ${LAPACK_LINKER_FLAGS})

# find_package(clapack CONFIG REQUIRED)
# find_package(OpenBLAS CONFIG REQUIRED)
# find_package(LAPACK REQUIRED)
# find_package(BLAS REQUIRED)
find_package(Python3 COMPONENTS Development)
find_package(pybind11 CONFIG REQUIRED)
find_package(doctest REQUIRED)
#find_package(Eigen3 CONFIG REQUIRED)
find_package(xtensor CONFIG REQUIRED)
#find_package(xtensor-blas CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)

add_library(va_cpp_static OBJECT)
pybind11_add_module(va_cpp SHARED $<TARGET_OBJECTS:va_cpp_static>)
add_executable(va_cpp_tests $<TARGET_OBJECTS:va_cpp_static>)

target_link_libraries(va_cpp_static PUBLIC
  ${BLAS_LIBRARIES}
  ${LAPACK_LIBRARIES}
  TBB::tbb
  xtensor-blas
  xtensor::use_xsimd
  xtensor::optimize
  xtensor::optimize
  xtensor::use_TBB
  pybind11::embed
  pybind11::module
  pybind11::pybind11
  doctest::doctest
  )

target_link_libraries(va_cpp PRIVATE
  ${BLAS_LIBRARIES}
  ${LAPACK_LIBRARIES}
  TBB::tbb
  )
target_link_libraries(va_cpp_tests PRIVATE
  ${BLAS_LIBRARIES}
  ${LAPACK_LIBRARIES}
  TBB::tbb
  pybind11::embed
  pybind11::module
  pybind11::pybind11
  doctest::doctest)

if (MSVC)
  target_link_libraries(va_cpp_tests PRIVATE
    pybind11::embed
    pybind11::module
    pybind11::pybind11
    )

  # By default all dynamic libraries on Windows land in ${CMAKE_BINARY_OUTPUT_DIRECTORY}, thus
  # we need to copy it to a desired location
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

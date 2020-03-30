function(cargo_build)
  cmake_parse_arguments(CARGO "" "NAME;LIB_TYPE;DST_PREFIX;DST_SUFFIX;BUILD_ARGS" "" ${ARGN})
  string(REPLACE "-" "_" LIB_NAME ${CARGO_NAME})

  set(CARGO_TARGET_DIR ${CMAKE_CURRENT_BINARY_DIR})

  set(CARGO_SRC_PREFIX ${CMAKE_${CARGO_LIB_TYPE}_LIBRARY_PREFIX})
  set(CARGO_SRC_SUFFIX ${CMAKE_${CARGO_LIB_TYPE}_LIBRARY_SUFFIX})

  if (NOT DEFINED CARGO_PREFIX AND NOT "DST_PREFIX" IN_LIST ARGN)
    set(CARGO_DST_PREFIX ${CMAKE_${CARGO_LIB_TYPE}_LIBRARY_PREFIX})
  endif()
  if (NOT DEFINED CARGO_SUFFIX AND NOT "DST_SUFFIX" IN_LIST ARGN)
    set(CARGO_DST_SUFFIX ${CMAKE_${CARGO_LIB_TYPE}_LIBRARY_SUFFIX})
  endif()

  if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(LIB_TARGET "x86_64-pc-windows-msvc")
    else()
      set(LIB_TARGET "i686-pc-windows-msvc")
    endif()
  elseif(ANDROID)
    if(ANDROID_SYSROOT_ABI STREQUAL "x86")
      set(LIB_TARGET "i686-linux-android")
    elseif(ANDROID_SYSROOT_ABI STREQUAL "x86_64")
      set(LIB_TARGET "x86_64-linux-android")
    elseif(ANDROID_SYSROOT_ABI STREQUAL "arm")
      set(LIB_TARGET "arm-linux-androideabi")
    elseif(ANDROID_SYSROOT_ABI STREQUAL "arm64")
      set(LIB_TARGET "aarch64-linux-android")
    endif()
  elseif(IOS)
    set(LIB_TARGET "universal")
  elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    set(LIB_TARGET "x86_64-apple-darwin")
  else()
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(LIB_TARGET "x86_64-unknown-linux-gnu")
    else()
      set(LIB_TARGET "i686-unknown-linux-gnu")
    endif()
  endif()

  if(NOT CMAKE_BUILD_TYPE)
    set(LIB_BUILD_TYPE "debug")
  elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(LIB_BUILD_TYPE "release")
  elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    set(LIB_BUILD_TYPE "release")
  else()
    set(LIB_BUILD_TYPE "debug")
  endif()

  set(LIB_FILENAME "${CARGO_SRC_PREFIX}${LIB_NAME}${CARGO_SRC_SUFFIX}")
  set(LIB_DST_FILENAME "${CARGO_DST_PREFIX}${LIB_NAME}${CARGO_DST_SUFFIX}")
  set(LIB_FILE "${CARGO_TARGET_DIR}/${LIB_TARGET}/${LIB_BUILD_TYPE}/${LIB_FILENAME}")
  set(LIB_DST_FILE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${LIB_DST_FILENAME}")
  set(DEPFILE_LIB_FILE "${CARGO_TARGET_DIR}/${LIB_TARGET}/${LIB_BUILD_TYPE}/${CARGO_PREFIX}${LIB_NAME}.d")

  # message(STATUS "LIB_FILE=${LIB_FILE}")
  # message(STATUS "LIB_DST_FILENAME=${LIB_DST_FILENAME}")
  # message(STATUS "CARGO_PREFIX=${CARGO_PREFIX}")
  # message(STATUS "CARGO_SUFFIX=${CARGO_SUFFIX}")

  if(IOS)
    set(CARGO_ARGS "lipo")
  else()
    set(CARGO_ARGS "build")
    list(APPEND CARGO_ARGS "--target" ${LIB_TARGET})
  endif()

  if(${LIB_BUILD_TYPE} STREQUAL "release")
    list(APPEND CARGO_ARGS "--release")
  endif()

  list(APPEND CARGO_ARGS ${CARGO_BUILD_ARGS})
  file(GLOB_RECURSE LIB_SOURCES "*.rs")

  set(CARGO_ENV_COMMAND ${CMAKE_COMMAND} -E env "CARGO_TARGET_DIR=${CARGO_TARGET_DIR}")

  if (NOT ${CMAKE_GENERATOR} STREQUAL "Ninja")
    # The DEPFILE parameter to add_custom_command only works with Ninja.
    add_custom_command(
      OUTPUT ${LIB_FILE} ${LIB_DST_FILE}
      COMMAND ${CARGO_ENV_COMMAND} ${CARGO_EXECUTABLE} ARGS ${CARGO_ARGS}
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIB_FILE} ${LIB_DST_FILE}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      DEPENDS ${LIB_SOURCES}
      COMMENT "running cargo")
  else()
    add_custom_command(
      OUTPUT ${LIB_FILE} ${LIB_DST_FILE}
      COMMAND ${CARGO_ENV_COMMAND} ${CARGO_EXECUTABLE} ARGS ${CARGO_ARGS}
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIB_FILE} ${LIB_DST_FILE}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      DEPENDS ${LIB_SOURCES}
      COMMENT "running cargo")
  endif()
  add_custom_target(${CARGO_NAME}_target ALL DEPENDS ${LIB_FILE})
  add_library(${CARGO_NAME} ${CARGO_LIB_TYPE} IMPORTED GLOBAL)
  add_dependencies(${CARGO_NAME} ${CARGO_NAME}_target)
  set_target_properties(${CARGO_NAME} PROPERTIES IMPORTED_LOCATION ${LIB_FILE})

endfunction()

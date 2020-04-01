

cmake_minimum_required(VERSION 3.12)

project(va_rs Rust)

if (WIN32)
  set(DST_SUFFIX ".pyd")
else()
  set(DST_SUFFIX ".so")
endif (WIN32)

cargo_build(
  NAME va_rs
  LIB_TYPE SHARED
  DST_PREFIX ""
  DST_SUFFIX "${DST_SUFFIX}"
  BUILD_ARGS "-Zfeatures=itarget"
  )

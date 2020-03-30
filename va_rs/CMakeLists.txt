

cmake_minimum_required(VERSION 3.12)

project(va_rs Rust)

cargo_build(
  NAME va_rs
  LIB_TYPE SHARED
  DST_PREFIX ""
  DST_SUFFIX ".pyd"
  BUILD_ARGS "-Zfeatures=itarget"
  )

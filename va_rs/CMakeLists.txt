

cmake_minimum_required(VERSION 3.17)

project(va_rs Rust)

cargo_build(
  NAME va_rs
  LIB_TYPE SHARED
  DST_PREFIX ""
  )

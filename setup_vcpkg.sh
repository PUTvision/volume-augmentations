#!/usr/bin/env bash
# Version 0.0.1
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg || exit
./bootstrap-vcpkg.sh
./vcpkg install "$@" pybind11 doctest xtensor xtensor[xsimd] xtensor[tbb] tbb xtensor-blas intel-mkl xsimd xtl

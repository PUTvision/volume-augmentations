# header-only library

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO przemkovv/xtensor-blas
    REF 5c5ce86dac69a37c2d29c0e1087df2441ea02f41
    SHA512 acc4dcdf4b235bb435762915420a8a97f7b9614f284eb289b214998d7e6e8eda96d6e06a0879def8689c9bcdd7c34b02ecde4c1167d611d1665e497911ea822e
    HEAD_REF master
)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    DISABLE_PARALLEL_CONFIGURE
    OPTIONS_RELEASE -DCXXBLAS_DEBUG=OFF
    OPTIONS_DEBUG -DCXXBLAS_DEBUG=ON
    OPTIONS
        -DXTENSOR_USE_FLENS_BLAS=OFF
        -DBUILD_TESTS=OFF
        -DBUILD_BENCHMARK=OFF
        -DUSE_OPENBLAS=ON
        -DDOWNLOAD_GTEST=OFF
        -DDOWNLOAD_GBENCHMARK=OFF
)

vcpkg_install_cmake()

vcpkg_fixup_cmake_targets(CONFIG_PATH lib/cmake/${PORT})

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug ${CURRENT_PACKAGES_DIR}/lib)
file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/include/xflens/cxxblas/netlib)

file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)

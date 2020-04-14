#pragma once

#include <xtensor/xfixed.hpp>
#include <xtensor/xtensor.hpp>

template <typename T, std::size_t N>
using vec = xt::xtensor_fixed<T, xt::xshape<N>>;

template <typename T, std::size_t Rows, std::size_t Cols>
using mat = xt::xtensor_fixed<T, xt::xshape<Rows, Cols>>;

using vec2f = vec<float, 2>;
using vec3f = vec<float, 3>;
using vec4f = vec<float, 4>;

using mat4f = mat<float, 4, 4>;
using mat3f = mat<float, 3, 3>;


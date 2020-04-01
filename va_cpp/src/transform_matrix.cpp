#include "transform_matrix.h"

#include <doctest/doctest.h>
#include <xtensor/xfixed.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>

auto translation_matrix(const vec3f &translation) -> mat4f
{
  mat4f m = xt::ones<float>(mat4f{}.shape());
  xt::view(m, xt::range(0, 3), 3) = translation;
  // m = xt::diag(translation);
  return m;
}

auto scale_matrix(const vec3f &s) -> mat4f
{
  mat4f m = xt::diag(s);
  // mat4f m = xt::ones<float>(mat4f{}.shape());
  // xt::view(xt::diagonal(m), xt::range(0,3)) = s;
  return m;
}

auto rotationX_matrix(const float angle) -> mat4f
{
  const auto sin = std::sin(angle);
  const auto cos = std::cos(angle);
  return {
      {1, 0, 0, 0},
      {0, cos, -sin, 0},
      {0, sin, cos, 0},
      {0, 0, 0, 1},
  };
}

auto rotationY_matrix(const float angle) -> mat4f
{
  const auto sin = std::sin(angle);
  const auto cos = std::cos(angle);
  return {
      {cos, 0, sin, 0},
      {0, 1, 0, 0},
      {-sin, 0, cos, 0},
      {0, 0, 0, 1},
  };
}

auto rotationZ_matrix(const float angle) -> mat4f
{
  const auto sin = std::sin(angle);
  const auto cos = std::cos(angle);
  return {
      {cos, -sin, 0, 0},
      {sin, cos, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
  };
}

auto shape_matrix(const vec4f &shape) -> mat4f
{
  mat4f m = xt::ones<float>(mat4f{}.shape());
  xt::view(m, xt::range(0, 3), 3) = shape;
  return m;
}

TEST_CASE("Translation matrix")
{
  const vec3f v{10, 20, 30};
  const mat4f expected = {
      {1, 0, 0, 10},
      {0, 1, 0, 20},
      {0, 0, 1, 30},
      {0, 0, 0, 1},
  };
  CHECK_EQ(translation_matrix(v), expected);
}
TEST_CASE("Scale matrix")
{
  const vec3f v{10, 20, 30};
  const mat4f expected = {
      {10, 0, 0, 0},
      {0, 20, 0, 0},
      {0, 0, 30, 0},
      {0, 0, 0, 1},
  };
  CHECK_EQ(scale_matrix(v), expected);
}
TEST_CASE("Shape matrix")
{
  const vec3f v{10, 20, 30};
  const mat4f expected = {
      {1, 0, 0, 10},
      {0, 1, 0, 20},
      {0, 0, 1, 30},
      {0, 0, 0, 1},
  };
  CHECK_EQ(shape_matrix(v), expected);
}

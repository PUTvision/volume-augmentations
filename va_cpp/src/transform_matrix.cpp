#include "transform_matrix.h"

#include <doctest/doctest.h>
#include <sstream>
#include <xtensor/xfixed.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>

auto translation_matrix(const vec3f &translation) -> mat4f
{
  return {
      {1, 0, 0, translation(0)},
      {0, 1, 0, translation(1)},
      {0, 0, 1, translation(2)},
      {0, 0, 0, 1},
  };
}

auto scale_matrix(const vec3f &s) -> mat4f
{
  return {
      {s(0), 0, 0, 0},
      {0, s(1), 0, 0},
      {0, 0, s(2), 0},
      {0, 0, 0, 1},
  };
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
  return translation_matrix(shape / 2.0f);
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
      {1, 0, 0, 5},
      {0, 1, 0, 10},
      {0, 0, 1, 15},
      {0, 0, 0, 1},
  };
  CHECK_EQ(shape_matrix(v), expected);
}

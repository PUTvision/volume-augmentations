#include "transform_matrix.h"

#include <doctest/doctest.h>
#include <sstream>
#include <xtensor-blas/xlinalg.hpp>
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

auto rotation_x_matrix(const float angle) -> mat4f
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

auto rotation_y_matrix(const float angle) -> mat4f
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

auto rotation_z_matrix(const float angle) -> mat4f
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
  return translation_matrix(shape / 2.0F);
}

auto Transform::rotate(const vec3f &angles) -> Transform &
{
  auto rotX = rotation_x_matrix(angles[0]);
  auto rotY = rotation_y_matrix(angles[1]);
  auto rotZ = rotation_z_matrix(angles[2]);
  using xt::linalg::dot;
  transformation = dot(dot(dot(transformation, rotX), rotY), rotZ);

  return *this;
}

auto Transform::translate(const vec3f &translation) -> Transform &
{
  using xt::linalg::dot;
  transformation = dot(transformation, translation_matrix(translation));
  return *this;
}

auto Transform::scale(const vec3f &s) -> Transform &
{
  using xt::linalg::dot;
  transformation = dot(transformation, scale_matrix(s));
  return *this;
}

Transform::Transform(const vec3f &shape) : transformation(shape_matrix(shape))
{}

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

TEST_CASE("Transform shape")
{

  const auto shape = vec3f{10.0F, 20.0F, 30.0F};
  const auto transform = Transform(shape);
  mat4f expected{
      {1, 0, 0, 5},
      {0, 1, 0, 10},
      {0, 0, 1, 15},
      {0, 0, 0, 1},
  };
  CHECK_EQ(transform.transformation, expected);
}

TEST_CASE("Transform scale")
{

  const auto transform = Transform(vec3f{1, 1, 1}).scale(vec3f{2, 1, 0.5F});
  mat4f expectedTransformation{
      {2.0F, 0.0F, 0.0F, 0.5F},
      {0.0F, 1.0F, 0.0F, 0.5F},
      {0.0F, 0.0F, 0.5F, 0.5F},
      {0.0F, 0.0F, 0.0F, 1.0F},
  };
  CHECK_EQ(transform.transformation, expectedTransformation);
}

TEST_CASE("Transform ")
{
  const auto shape = vec3f{10.0F, 20.0F, 30.0F};
  const auto rotation = vec3f{3.14F, 3.14F, 3.14F / 2};
  const auto translation = vec3f{4, 3, 2};
  const auto scale = vec3f{2, 1, 0.5};

  [[maybe_unused]] const auto transform =
      Transform(shape).rotate(rotation).translate(translation).scale(scale);
}

TEST_CASE("Tensor slicing")
{}


#pragma once

#include "types.h"

auto translation_matrix(const vec3f &translation) -> mat4f;
auto scale_matrix(const vec3f &s) -> mat4f;
auto rotation_x_matrix(float angle) -> mat4f;
auto rotation_y_matrix(float angle) -> mat4f;
auto rotation_z_matrix(float angle) -> mat4f;
auto shape_matrix(const vec4f &shape) -> mat4f;

struct Transform
{
  mat4f transformation;

  auto rotate(const vec3f &angles) -> Transform &;
  auto translate(const vec3f &translation) -> Transform &;
  auto scale(const vec3f &s) -> Transform &;
  explicit Transform(const vec3f &shape);
};

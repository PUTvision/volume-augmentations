#pragma once

#include "types.h"

auto translation_matrix(const vec3f &translation) -> mat4f;
auto scale_matrix(const vec3f &s) -> mat4f;
auto rotationX_matrix(float angle) -> mat4f;
auto rotationY_matrix(float angle) -> mat4f;
auto rotationZ_matrix(float angle) -> mat4f;
auto shape_matrix(const vec4f &shape) -> mat4f;

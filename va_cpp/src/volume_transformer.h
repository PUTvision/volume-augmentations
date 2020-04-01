#pragma once

#include "transform_matrix.h"
#include "types.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <vector>
#include <xtensor-blas/xlinalg.hpp>
#include <xtensor/xfixed.hpp>

struct Transform
{
  mat4f transformation;

  auto rotate(const vec3f &angles) -> Transform &
  {
    auto rotX = rotationX_matrix(angles[0]);
    auto rotY = rotationY_matrix(angles[1]);
    auto rotZ = rotationZ_matrix(angles[2]);
    using xt::linalg::dot;
    transformation = dot(dot(dot(transformation, rotX), rotY), rotZ);
    // clang-format off
    
    // transformation = transformation
      // * rotationX_matrix(angles[0])
      // * rotationY_matrix(angles[1])
      // * rotationZ_matrix(angles[2]);
    // clang-format on

    return *this;
  }

  auto translate(const vec3f &translation) -> Transform &
  {
    // transformation *= translation_matrix(translation);
    return *this;
  }

  auto scale(const vec3f &s) -> Transform &
  {
    transformation *= scale_matrix(s);
    return *this;
  }

  explicit Transform(const vec3f &shape) : transformation(shape_matrix(shape))
  {}
};

struct VolumeTransformer
{

  std::vector<float> volumeData;
  // Eigen::TensorMap<float, 4> volume;
};



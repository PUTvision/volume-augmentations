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

    return *this;
  }

  auto translate(const vec3f &translation) -> Transform &
  {
    using xt::linalg::dot;
    transformation = dot(transformation, translation_matrix(translation));
    return *this;
  }

  auto scale(const vec3f &s) -> Transform &
  {
    using xt::linalg::dot;
    transformation = dot(transformation, scale_matrix(s));
    return *this;
  }

  explicit Transform(const vec3f &shape) : transformation(shape_matrix(shape))
  {}
};

enum class InterpolationType
{
  Linear,
  Nearest,
};

struct VolumeTransformer
{

  using Transformation = int;
  std::size_t volume_count_;
  std::vector<float> volume_data{};
  std::vector<Transformation> transformations_{};
  std::vector<InterpolationType> interpolation_types_{};

  VolumeTransformer(std::size_t volume_count) : volume_count_(volume_count)
  {}
  template <typename... Transformations>
  auto compose(const Transformations &... transformations)
  {
    transformations_ = {transformations...};
  }

  auto compose(const std::vector<Transformation> & /*transformations*/) const
  {}

  auto rotate()
  {}
  [[nodiscard]] auto translate(const vec3f &) const -> Transformation
  {
    return {};
  };
  [[nodiscard]] auto declare_shape(const vec3f &) const -> Transformation
  {
    return {};
  };
  [[nodiscard]] auto scale(const vec3f &) const -> Transformation
  {
    return {};
  };
  [[nodiscard]] auto use_interpolation(InterpolationType interpolation)
      -> Transformation
  {
    interpolation_types_ = {interpolation};
    return {};
  }
  [[nodiscard]] auto
  use_interpolation(std::vector<InterpolationType> interpolations)
      -> Transformation
  {
    interpolation_types_ = std::move(interpolations);
    return {};
  };
};


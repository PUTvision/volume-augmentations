#pragma once

#include "transform_matrix.h"
#include "types.h"

#include <fmt/printf.h>
#include <iostream>
#include <variant>
#include <vector>

enum class ComposableType
{
  ParametersSetter,
  Affine
};

template <typename T>
struct Range
{
  T min;
  T max;
};
template <typename T, typename Tag>
struct Ranges
{
  Range<T> x;
  Range<T> y;
  Range<T> z;
};

struct rotation_ranges_tag;
struct translation_ranges_tag;

template <typename T>
using RotationRanges = Ranges<T, rotation_ranges_tag>;

template <typename T>
using TranslationRanges = Ranges<T, translation_ranges_tag>;

enum class InterpolationType
{
  NearestNeighbour,
  Linear
};

template <typename DerivedType>
struct Composable
{
  static constexpr ComposableType type = DerivedType::type;

  template <typename OtherType>
  constexpr static auto is_stackable_with(OtherType) -> bool
  {
    return type == DerivedType::type;
  }
};

struct UseDefaultSettings : Composable<UseDefaultSettings>
{
  static constexpr ComposableType type = ComposableType::ParametersSetter;
};

struct SetInterpolation : Composable<SetInterpolation>
{
  static constexpr ComposableType type = ComposableType::ParametersSetter;

  SetInterpolation(std::initializer_list<InterpolationType> interpolations)
      : interpolations_{interpolations}
  {}
  explicit SetInterpolation(std::vector<InterpolationType> interpolations)
      : interpolations_{std::move(interpolations)}
  {}

  std::vector<InterpolationType> interpolations_;
};

struct SetProbabilityOfApplication : Composable<SetProbabilityOfApplication>
{
  static constexpr ComposableType type = ComposableType::ParametersSetter;

  SetProbabilityOfApplication(std::vector<float> probabilities)
      : probabilities_{std::move(probabilities)}
  {}

  std::vector<float> probabilities_;
};

struct SetFillValue : Composable<SetFillValue>
{
  static constexpr ComposableType type = ComposableType::ParametersSetter;

  SetFillValue(std::initializer_list<float> fill_values)
      : fill_values_{fill_values}
  {}
  explicit SetFillValue(std::vector<float> fill_values)
      : fill_values_{std::move(fill_values)}
  {}

  std::vector<float> fill_values_;
};

struct RandomRotation : Composable<RandomRotation>
{
  static constexpr ComposableType type = ComposableType::Affine;

  RandomRotation(std::initializer_list<RotationRanges<float>> rotation_limits)
      : rotation_limits_{rotation_limits}
  {}
  explicit RandomRotation(std::vector<RotationRanges<float>> rotation_limits)
      : rotation_limits_{std::move(rotation_limits)}
  {}

  auto foo() -> void
  {
    rotation_limits_ = {};
  }

  std::vector<RotationRanges<float>> rotation_limits_;
};

struct RandomTranslation : Composable<RandomTranslation>
{
  static constexpr ComposableType type = ComposableType::Affine;

  RandomTranslation(
      std::initializer_list<TranslationRanges<float>> translation_limits)
      : translation_limits_{translation_limits}
  {}
  explicit RandomTranslation(
      std::vector<TranslationRanges<float>> translation_limits)
      : translation_limits_{std::move(translation_limits)}
  {}
  auto foo() -> void
  {
    translation_limits_ = {};
  }

  std::vector<TranslationRanges<float>> translation_limits_;
};

using AnyComposable = std::variant<UseDefaultSettings,
                                   SetInterpolation,
                                   SetProbabilityOfApplication,
                                   SetFillValue,
                                   RandomRotation,
                                   RandomTranslation>;

struct Compose
{
  template <typename... Composable>
  Compose(Composable... transformations) : transformations_{transformations...}
  {}
  explicit Compose(std::vector<AnyComposable> transformations)
      : transformations_{std::move(transformations)}
  {}
  void call()
  {
    for (const auto &transformation : transformations_) {
      std::visit(
          []<typename T>(const T &) {
            fmt::print("Transformation: {}\n", typeid(T).name());
          },
          transformation);
    }
  }

private:
  std::vector<AnyComposable> transformations_;
};


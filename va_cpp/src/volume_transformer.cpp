#include "volume_transformer.h"

#include <doctest/doctest.h>
#include <xflens/cxxblas/level3/gemm.tcc>

TEST_CASE("Transform shape")
{

  const auto shape = vec3f{10.0F, 20.0F, 30.0F};
  const auto transform = Transform(shape);
  mat4f expected{
      {1, 0, 20, 5},
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

  const auto transform =
      Transform(shape).rotate(rotation).translate(translation).scale(scale);
}

TEST_CASE("Tensor slicing")
{}

TEST_CASE("Volume transformer")
{

  VolumeTransformer t{1};
  const auto shape = vec3f{5, 5, 5};
  const auto translation = vec3f{2, 2, 0};
  t.compose(t.declare_shape(shape), t.translate(translation));
}


#include "volume_transformer.h"

#include <doctest/doctest.h>
#include <xflens/cxxblas/level3/gemm.tcc>


TEST_CASE("Transform shape")
{

  const auto transform = Transform(vec3f{10.0f, 20.0f, 30.0f});
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

  const auto transform = Transform(vec3f{1, 1, 1}).scale(vec3f{1, 1, 1.0f});
  mat4f expectedTransformation{
      {2, 0, 0, 1},
      {0, 1, 0, 1},
      {0, 0, 1.0f, 1},
      {0, 0, 0, 1},
  };
  CHECK_EQ(transform.transformation, expectedTransformation);
}

TEST_CASE("Transform ")
{
  const auto transform = Transform(vec4f{1.0f, 10.0f, 20.0f, 30.0f})
                             .rotate({3.14f, 3.14f, 3.14f / 2})
                             .translate({4, 3, 2})
                             .scale({2, 1, 0.5});
}

TEST_CASE("Tensor slicing")
{}


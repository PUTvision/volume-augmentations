#include <Eigen/Dense>
#include <array>
#include <cmath>
#include <doctest/doctest.h>
#include <vector>

auto deg2rad(float deg) -> float
{
  return deg * static_cast<float>(M_PI) / 180.0f;
}
auto rad2deg(float rad) -> float
{
  return rad / static_cast<float>(M_PI) * 180.0f;
}
auto deg2rad(Eigen::Vector3f deg)
{
  return (deg.array() * static_cast<float>(M_PI) / 180.0f).matrix();
}
auto rad2deg(Eigen::Vector3f rad)
{
  return (rad.array() / static_cast<float>(M_PI) * 180.0f).matrix();
}

namespace operations {

auto translation_matrix(const Eigen::Vector3f &translation)
{
  Eigen::Matrix4f m = Eigen::Matrix4f::Identity();
  m.block<3, 1>(0, 3) = translation;
  return m;
}

auto scale_matrix(const Eigen::Vector3f &s)
{
  Eigen::Matrix4f m = Eigen::Matrix4f::Identity();
  m.diagonal().head(3) = s;
  return m;
}

auto rotationX_matrix(const float angle)
{
  const auto sin = std::sin(angle);
  const auto cos = std::cos(angle);
  Eigen::Matrix4f m;
  // clang-format off
  m <<
    1, 0,   0,    0,
    0, cos, -sin, 0,
    0, sin, cos,  0,
    0, 0,   0,    1;
  // clang-format on
  return m;
}

auto rotationY_matrix(const float angle)
{
  const auto sin = std::sin(angle);
  const auto cos = std::cos(angle);
  Eigen::Matrix4f m;
  // clang-format off
  m <<
    cos,  0, sin, 0,
    0,    1, 0,   0,
    -sin, 0, cos, 0,
    0,    0, 0,   1;
  // clang-format on
  return m;
}

auto rotationZ_matrix(const float angle)
{
  const auto sin = std::sin(angle);
  const auto cos = std::cos(angle);
  Eigen::Matrix4f m;
  // clang-format off
  m <<
    cos, -sin, 0, 0,
    sin, cos,  0, 0,
    0,   0,    1, 0,
    0,   0,    0, 1;
  // clang-format on
  return m;
}

auto shape_matrix(const Eigen::Vector4f &shape)
{
  Eigen::Matrix4f m = Eigen::Matrix4f::Identity();
  m.block<3, 1>(0, 3) = shape.tail<3>();
  return m;
}

} // namespace operations

struct Transform
{
  Eigen::Matrix4f transformation;

  auto rotate(const Eigen::Vector3f &angles) -> Transform &
  {
    using operations::rotationX_matrix;
    using operations::rotationY_matrix;
    using operations::rotationZ_matrix;
    // clang-format off
    transformation = transformation
      * rotationX_matrix(angles[0])
      * rotationY_matrix(angles[1])
      * rotationZ_matrix(angles[2]);
    // clang-format on

    return *this;
  }

  auto translate(const Eigen::Vector3f &translation) -> Transform &
  {
    transformation *= operations::translation_matrix(translation);
    return *this;
  }

  auto scale(const Eigen::Vector3f &s) -> Transform &
  {
    transformation *= operations::scale_matrix(s);
    return *this;
  }

  explicit Transform(const Eigen::Vector4f &shape)
      : transformation(operations::shape_matrix(shape))
  {}
};

TEST_CASE("Transform shape")
{

  const auto transform = Transform(Eigen::Vector4f{1.0f, 10.0f, 20.0f, 30.0f});
  Eigen::Matrix4f expectedTransformation;
  // clang-format off
  expectedTransformation <<
    1, 0, 0, 10,
    0, 1, 0, 20,
    0, 0, 1, 30,
    0, 0, 0, 1;
  // clang-format on
  CHECK_EQ(transform.transformation, expectedTransformation);
}

TEST_CASE("Transform scale")
{

  const auto transform = Transform(Eigen::Vector4f::Ones()).scale({2, 1, 0.5f});
  Eigen::Matrix4f expectedTransformation;
  // clang-format off
  expectedTransformation <<
    2, 0, 0, 1,
    0, 1, 0, 1,
    0, 0, 0.5f, 1,
    0, 0, 0, 1;
  // clang-format on
  CHECK_EQ(transform.transformation, expectedTransformation);
}

TEST_CASE("Transform ")
{
  const auto transform = Transform(Eigen::Vector4f{1.0f, 10.0f, 20.0f, 30.0f})
                             .rotate({3.14f, 3.14f, 3.14f / 2})
                             .translate({4, 3, 2})
                             .scale({2, 1, 0.5});
}


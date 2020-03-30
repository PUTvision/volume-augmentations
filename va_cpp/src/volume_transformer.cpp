#include <doctest/doctest.h>

#include <array>
#include <vector>

#include <Eigen/Dense>

struct Transform {

  Eigen::Matrix4f transformation = Eigen::Matrix4f::Identity();

  Transform rotate(const Eigen::Vector3f &rotation) const { return *this; }
  Transform translate(const Eigen::Vector3f &translation) const {
    return *this;
  }
  Transform scale(const Eigen::Vector3f &s) {

    Eigen::Matrix4f s2 = Eigen::Matrix4f::Identity();
    s2.diagonal().head(3) = s;
    transformation *= s2;
    return *this;
  }

  Eigen::Matrix4f
  get_shaped_transformation(const Eigen::Vector4f &shape) const {
    Eigen::Matrix4f m = Eigen::Matrix4f::Identity();
    m.block<3, 1>(0, 3) = shape.tail<3>();
    return m;
  }

  explicit Transform(const Eigen::Vector4f &shape)
      : transformation(get_shaped_transformation(shape)) {}
};


TEST_CASE("Transform shape") {

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

TEST_CASE("Transform scale") {

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

TEST_CASE("Transform ") {
  const auto transform = Transform(Eigen::Vector4f{1.0f, 10.0f, 20.0f, 30.0f})
                             .rotate({3.14f, 3.14f, 3.14f / 2})
                             .translate({4, 3, 2})
                             .scale({2, 1, 0.5});
}


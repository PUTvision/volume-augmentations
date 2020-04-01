#include "utils.h"

#include <Eigen/Dense>
#include <doctest/doctest.h>

auto deg2rad(float deg) -> float
{
  return deg * static_cast<float>(M_PI) / 180.0f;
}
auto rad2deg(float rad) -> float
{
  return rad / static_cast<float>(M_PI) * 180.0f;
}

TEST_CASE("deg2rad float")
{
  CHECK(deg2rad(1) == doctest::Approx(0.017453f));
  CHECK(deg2rad(45) == doctest::Approx(0.785398f));
  CHECK(deg2rad(90) == doctest::Approx(1.570796f));
  CHECK(deg2rad(-45) == doctest::Approx(-0.785398f));
}
TEST_CASE("rad2deg float")
{
  CHECK(rad2deg(1) == doctest::Approx(57.2957f));
  CHECK(rad2deg(-1) == doctest::Approx(-57.2957f));
  CHECK(rad2deg(3.1415) == doctest::Approx(179.994f));
}
TEST_CASE("deg2rad Vector3f")
{
  Eigen::Vector3f v;
  v << 1, 45, 90;
  Eigen::Vector3f expected;
  expected << 0.017453f, 0.785398f, 1.570796f;
  const auto result = deg2rad(v);
  CHECK(result(0) == doctest::Approx(expected(0)));
  CHECK(result(1) == doctest::Approx(expected(1)));
  CHECK(result(2) == doctest::Approx(expected(2)));
}
TEST_CASE("deg2red float")
{
  Eigen::Vector3f v;
  v << 0.017453f, 0.785398f, 1.570796f;
  Eigen::Vector3f expected;
  expected << 1, 45, 90;
  const auto result = rad2deg(v);
  CHECK(result(0) == doctest::Approx(expected(0)));
  CHECK(result(1) == doctest::Approx(expected(1)));
  CHECK(result(2) == doctest::Approx(expected(2)));
}

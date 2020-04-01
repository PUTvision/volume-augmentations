#pragma once

#include <Eigen/Dense>

auto deg2rad(float deg) -> float;
auto rad2deg(float rad) -> float;

auto deg2rad(const Eigen::Vector3f &deg)
{
  return (deg.array() * static_cast<float>(M_PI) / 180.0f).matrix();
}
auto rad2deg(const Eigen::Vector3f &rad)
{
  return (rad.array() / static_cast<float>(M_PI) * 180.0f).matrix();
}


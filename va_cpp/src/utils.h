#pragma once

template <typename T>
inline constexpr T pi_v = static_cast<T>(3.14159265358979323846);


auto deg2rad(float deg) -> float;
auto rad2deg(float rad) -> float;

/*
auto deg2rad(const Eigen::Vector3f &deg)
{
  return (deg.array() * static_cast<float>(M_PI) / 180.0f).matrix();
}
auto rad2deg(const Eigen::Vector3f &rad)
{
  return (rad.array() / static_cast<float>(M_PI) * 180.0f).matrix();
}

*/

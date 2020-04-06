

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <tuple>
#include <vector>

namespace py = pybind11;

class Transformation
{
public:
  [[nodiscard]] virtual auto is_stackable() const -> bool = 0;
  virtual auto print() const -> void = 0;
  Transformation() = default;
  Transformation(const Transformation &) = delete;
  auto operator=(const Transformation &) -> Transformation & = delete;
  auto operator=(Transformation &&) -> Transformation & = default;
  Transformation(Transformation &&) = default;
  virtual ~Transformation() = default;
};

class RandomRotation : public Transformation
{
public:
  using RotationRanges = std::
      tuple<std::tuple<int, int>, std::tuple<int, int>, std::tuple<int, int>>;

  explicit RandomRotation(RotationRanges rotation_limits)
      : _rotation_limits{std::move(rotation_limits)}
  {}

  [[nodiscard]] auto is_stackable() const -> bool override
  {
    return true;
  }

  auto print() const -> void override
  {
    std::cout << "RandomRotation" << std::endl;
  }
  // virtual ~RandomRotation() = default;

private:
  RotationRanges _rotation_limits;
};

class Compose
{
public:
  explicit Compose(std::vector<std::shared_ptr<Transformation>> transformations)
      : _transformations{std::move(transformations)}
  {}

  auto print() const -> void
  {
    for (const auto &transformation : _transformations) {
      transformation->print();
    }
  }

private:
  std::vector<std::shared_ptr<Transformation>> _transformations;
};

// NOLINTNEXTLINE
PYBIND11_MODULE(va_cpp, m)
{
  m.doc() = "pybind11 example plugin"; // optional module docstring

  // NOLINTNEXTLINE
  py::class_<Transformation, std::shared_ptr<Transformation>>(m,
                                                              "Transformation");

  // NOLINTNEXTLINE
  py::class_<RandomRotation, Transformation, std::shared_ptr<RandomRotation>>(
      m,
      "RandomRotation")
      .def(py::init<RandomRotation::RotationRanges>());

  // NOLINTNEXTLINE
  py::class_<Compose>(m, "Compose")
      .def(py::init<std::vector<std::shared_ptr<Transformation>>>())
      .def("print", &Compose::print);
}

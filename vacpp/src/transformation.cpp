
#include <vector>
#include <tuple>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class Transformation {
public:
    [[nodiscard]] virtual bool is_stackable() const = 0;
    virtual void print() const = 0;
};

class RandomRotation : public Transformation {
public:
    using RotationRanges = std::tuple<std::tuple<int, int>, std::tuple<int, int>, std::tuple<int, int>>;

    explicit RandomRotation(RotationRanges rotation_limits) : _rotation_limits{std::move(rotation_limits)} {

    }

    [[nodiscard]] bool is_stackable() const override {
        return true;
    }

    void print() const override {
        std::cout << "RandomRotation" << std::endl;
    }

private:
    RotationRanges _rotation_limits;
};

class Compose {
public:
    explicit Compose(std::vector<std::shared_ptr<Transformation>> transformations) : _transformations{std::move(transformations)} {

    }

    void print() {
        for (const auto &transformation : _transformations) {
            transformation->print();
        }
    }

private:
    std::vector<std::shared_ptr<Transformation>> _transformations;
};

PYBIND11_MODULE(vacpp, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    py::class_<Transformation, std::shared_ptr<Transformation>>(m, "Transformation");

    py::class_<RandomRotation, Transformation, std::shared_ptr<RandomRotation>>(m, "RandomRotation")
            .def(py::init<RandomRotation::RotationRanges>());

    py::class_<Compose>(m, "Compose")
            .def(py::init<std::vector<std::shared_ptr<Transformation>>>())
            .def("print", &Compose::print);
}

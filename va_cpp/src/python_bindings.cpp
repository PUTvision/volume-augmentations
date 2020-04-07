
#include "python_bindings.h"

#include "volume_transformer.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

// NOLINTNEXTLINE
PYBIND11_MODULE(va_cpp, m)
{
  m.doc() = "pybind11 example plugin";

  py::enum_<InterpolationType>(m, "InterpolationType")
      .value("NearestNeighbour", InterpolationType::NearestNeighbour)
      .value("Linear", InterpolationType::Linear)
      .export_values();

  py::class_<UseDefaultSettings>(m, "UseDefaultSettings").def(py::init());

  py::class_<SetInterpolation>(m, "SetInterpolation")
      .def(py::init<std::vector<InterpolationType>>());

  py::class_<SetProbabilityOfApplication>(m, "SetProbabilityOfApplication")
      .def(py::init<std::vector<float>>());

  py::class_<SetFillValue>(m, "SetFillValue")
      .def(py::init<std::vector<float>>());

  {
    using R = Range<float>;
    py::class_<R>(m, "Range")
        .def(py::init<float, float>())
        .def_readwrite("min", &R::min)
        .def_readwrite("max", &R::max);
  }
  {
    using RotRng = RotationRanges<float>;
    using Rng = Range<float>;
    py::class_<RotRng>(m, "RotationRanges")
        .def(py::init<Rng, Rng, Rng>())
        .def_readwrite("x", &RotRng::x)
        .def_readwrite("y", &RotRng::y)
        .def_readwrite("z", &RotRng::z);
  }
  {
    using TrRng = TranslationRanges<float>;
    using Rng = Range<float>;
    py::class_<TrRng>(m, "TranslationRanges")
        .def(py::init<Rng, Rng, Rng>())
        .def_readwrite("x", &TrRng::x)
        .def_readwrite("y", &TrRng::y)
        .def_readwrite("z", &TrRng::z);
  }
  py::class_<RandomRotation>(m, "RandomRotation")
      .def(py::init<std::vector<RotationRanges<float>>>());
  py::class_<RandomTranslation>(m, "RandomTranslation")
      .def(py::init<std::vector<TranslationRanges<float>>>());
  py::class_<Compose>(m, "Compose")
      .def(py::init<std::vector<AnyComposable>>())
      .def("__call__", &Compose::call);
}

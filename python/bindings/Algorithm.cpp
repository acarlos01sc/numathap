#include <pybind11/pybind11.h>
#include <numathap/numathap.hpp>

namespace py = pybind11;

namespace numathap::python {

void bindAlgorithm(py::module_& m) {
  py::enum_<numathap::backend::integrate::Algorithm>(m, "Algorithm",
      R"pbdoc(
      Numerical integration algorithm to use.

      Pass one of these values to :func:`configure` to build a
      MathEnvironment that uses that algorithm's default settings, or
      use :class:`AdaptiveSimpsonConfig`/:class:`GaussKronrod15Config`
      via :func:`configure` for fine-grained control.
      )pbdoc")
      .value("AdaptiveSimpson",
          numathap::backend::integrate::Algorithm::AdaptiveSimpson,
          R"pbdoc(
          Adaptive Simpson's rule — recursively subdivides the interval
          until a target tolerance is met. Configurable via
          :class:`AdaptiveSimpsonConfig`.
          )pbdoc")
      .value("GaussKronrod15",
          numathap::backend::integrate::Algorithm::GaussKronrod15,
          R"pbdoc(
          Gauss-Kronrod 15-point quadrature. Configurable via
          :class:`GaussKronrod15Config`.
          )pbdoc")
      .export_values();
}

}  // namespace numathap::python
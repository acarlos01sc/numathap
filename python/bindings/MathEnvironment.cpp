#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <numathap/numathap.hpp>

namespace py = pybind11;

namespace numathap::python {

void bindMathEnvironment(py::module_& m) {
  // MathLibrary
  py::enum_<numathap::MathLibrary>(m, "MathLibrary",
                                    R"pbdoc(
      Underlying math library backing a MathEnvironment's numeric
      operations.
      )pbdoc")
      .value("CMath", numathap::MathLibrary::CMath,
             R"pbdoc(
      Standard C math library (<cmath>).
      )pbdoc")
      .export_values();

  // NumericType
  py::enum_<numathap::NumericType>(m, "NumericType",
                                    R"pbdoc(
      Numeric representation used internally by a MathEnvironment when
      evaluating expressions.
      )pbdoc")
      .value("Double", numathap::NumericType::Double,
             R"pbdoc(
      Double-precision floating point.
      )pbdoc")
      .export_values();

  // MathEnvironment
  py::class_<numathap::MathEnvironment>(m, "MathEnvironment",
                                         R"pbdoc(
      Configuration bundle controlling how expressions are evaluated,
      integrated and differentiated.

      Built via :func:`configure` rather than constructed directly in
      most cases — it selects capabilities (see :class:`Capability`)
      and, when relevant, the integration algorithm (see
      :class:`Algorithm`). Passed as the optional last argument to
      :func:`prepare`, :func:`integrate` and :func:`differentiate`.
      )pbdoc")
      .def(py::init<>(),
           R"pbdoc(
      Create a MathEnvironment with default settings (equivalent
      to calling :func:`configure` with no arguments).
      )pbdoc")
      .def_property_readonly("math_library",
                              &numathap::MathEnvironment::mathLibrary,
                              R"pbdoc(
      MathLibrary: Active mathematical library.
      )pbdoc")
      .def_property_readonly("numeric_type",
                              &numathap::MathEnvironment::numericType,
                              R"pbdoc(
      NumericType: Active numeric type.
      )pbdoc")
      .def_property_readonly(
          "integration_algorithm",
          &numathap::MathEnvironment::integrationAlgorithm,
          R"pbdoc(
      Algorithm: Currently selected integration algorithm (see
      :class:`Algorithm`).
      )pbdoc")
      .def_property_readonly(
          "integration_algorithm_config",
          &numathap::MathEnvironment::integrationAlgorithmConfig,
          R"pbdoc(
      AdaptiveSimpsonConfig | GaussKronrod15Config: Configuration
      object for the currently selected integration algorithm — the
      concrete type matches :attr:`integration_algorithm`.
      )pbdoc")
      .def(
          "enable_capability",
          &numathap::MathEnvironment::enableCapability, py::arg("capability"),
          R"pbdoc(
      Enables a processing capability on this environment.

      Args:
          capability: The capability to enable (see
              :class:`Capability`).
      )pbdoc")
      .def(
          "disable_capability",
          &numathap::MathEnvironment::disableCapability,
          py::arg("capability"),
          R"pbdoc(
      Disables a processing capability on this environment.

      Args:
          capability: The capability to disable (see
              :class:`Capability`).
      )pbdoc")
      .def(
          "has_capability", &numathap::MathEnvironment::hasCapability,
          py::arg("capability"),
          R"pbdoc(
      Checks whether a capability is enabled on this environment.

      Args:
          capability: The capability to query (see
              :class:`Capability`).

      Returns:
          bool: True if ``capability`` is enabled.
      )pbdoc")
      .def_property_readonly(
          "series_type", &numathap::MathEnvironment::seriesType,
          R"pbdoc(
      SeriesType: Currently selected series expansion type (see
      :class:`SeriesType`).
      )pbdoc")
      .def_property_readonly(
          "series_config", &numathap::MathEnvironment::seriesConfig,
          R"pbdoc(
      TaylorConfig: Configuration object for the currently selected
      series type — the concrete type matches :attr:`series_type`.
      )pbdoc");

  // NOTE: MathEnvironment::mathAdapter() is intentionally not exposed —
  // it is an implementation detail, per explicit decision.
}

}  // namespace numathap::python
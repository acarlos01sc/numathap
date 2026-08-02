#include <pybind11/pybind11.h>
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
          )pbdoc");
}

}  // namespace numathap::python
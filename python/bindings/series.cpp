/**
 * @file series.cpp
 * @brief Python bindings for series generation.
 */

#include <pybind11/pybind11.h>

#include <numathap/numathap.hpp>

namespace py = pybind11;

namespace numathap::python {

void bindSeries(py::module_& m)
{
  // SeriesType
  py::enum_<numathap::backend::series::SeriesType>(m, "SeriesType",
                                                     R"pbdoc(
      Series expansion type to use.

      Pass one of these values to :func:`configure` to build a
      MathEnvironment that uses that series type's default settings, or
      use :class:`TaylorConfig` via :func:`configure` for fine-grained
      control.
      )pbdoc")
      .value("Taylor", numathap::backend::series::SeriesType::Taylor,
             R"pbdoc(
      Taylor series expansion. Configurable via :class:`TaylorConfig`.
      )pbdoc")
      .export_values();

  // Taylor series configuration
  py::class_<numathap::backend::series::TaylorConfig>(m, "TaylorConfig",
                                                        R"pbdoc(
      Tuning parameters for the Taylor series expansion.

      Pass an instance to :func:`configure` to build a MathEnvironment
      that expands series using these settings.
      )pbdoc")
      .def(py::init<>(),
           R"pbdoc(
      Create a TaylorConfig with the default expansion order.
      )pbdoc")
      .def_readwrite("order", &numathap::backend::series::TaylorConfig::order,
                      R"pbdoc(
      int: Number of terms of the expansion (i.e. the highest degree
      retained). Higher values mean a more accurate approximation at
      the cost of more computation.
      )pbdoc");

  // series(expression, variable, center) — default environment
  m.def(
      "series",
      [](const numathap::PreparedAst& expression,
         const std::string& variable,
         const std::string& center) {
        return numathap::series(expression, variable, center);
      },
      py::arg("expression"),
      py::arg("variable"),
      py::arg("center"),
      R"pbdoc(
      Build a series expansion using the default environment.

      Generates a series expansion around the specified center. The
      expression must have gone through :func:`prepare` first.

      Args:
          expression: A previously prepared expression (see
              :func:`prepare`).
          variable: Name of the expansion variable.
          center: Expansion center expression, such as ``"0"`` or
              ``"pi/2"``.

      Returns:
          PreparedAst: A new prepared expression containing the series
          expansion.
      )pbdoc");

  // series(expression, variable, center, context) — custom context
  m.def(
      "series",
      [](const numathap::PreparedAst& expression,
         const std::string& variable,
         const std::string& center,
         const numathap::Context& context) {
        return numathap::series(expression, variable, center, context);
      },
      py::arg("expression"),
      py::arg("variable"),
      py::arg("center"),
      py::arg("context"),
      R"pbdoc(
      Build a series expansion using a custom context.

      The context is used to resolve symbols required to evaluate the
      expansion center or other values involved during series
      generation.

      Args:
          expression: A previously prepared expression (see
              :func:`prepare`).
          variable: Name of the expansion variable.
          center: Expansion center expression.
          context: Context containing symbol values.

      Returns:
          PreparedAst: A new prepared expression containing the series
          expansion.
      )pbdoc");

  // series(expression, variable, center, environment) — custom environment
  m.def(
      "series",
      [](const numathap::PreparedAst& expression,
         const std::string& variable,
         const std::string& center,
         const numathap::MathEnvironment& environment) {
        return numathap::series(expression, variable, center, environment);
      },
      py::arg("expression"),
      py::arg("variable"),
      py::arg("center"),
      py::arg("environment"),
      R"pbdoc(
      Build a series expansion using a custom environment.

      The environment defines the active series type (see
      :class:`SeriesType`) and its configuration (see
      :class:`TaylorConfig`).

      Args:
          expression: A previously prepared expression (see
              :func:`prepare`).
          variable: Name of the expansion variable.
          center: Expansion center expression.
          environment: Environment containing series configuration.

      Returns:
          PreparedAst: A new prepared expression containing the series
          expansion.
      )pbdoc");

  // series(expression, variable, center, context, environment) — full control
  m.def(
      "series",
      [](const numathap::PreparedAst& expression,
         const std::string& variable,
         const std::string& center,
         const numathap::Context& context,
         const numathap::MathEnvironment& environment) {
        return numathap::series(expression, variable, center, context,
                                 environment);
      },
      py::arg("expression"),
      py::arg("variable"),
      py::arg("center"),
      py::arg("context"),
      py::arg("environment"),
      R"pbdoc(
      Build a series expansion using a custom context and environment.

      Provides full control over symbol resolution (via ``context``)
      and series configuration (via ``environment``).

      Args:
          expression: A previously prepared expression (see
              :func:`prepare`).
          variable: Name of the expansion variable.
          center: Expansion center expression.
          context: Context containing symbol values.
          environment: Environment containing series configuration.

      Returns:
          PreparedAst: A new prepared expression containing the series
          expansion.
      )pbdoc");
}

} // namespace numathap::python
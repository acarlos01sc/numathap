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
        Build a symbolic series expansion of a prepared expression.

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
}

}  // namespace numathap::python
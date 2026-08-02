#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <numathap/numathap.hpp>

namespace py = pybind11;

namespace numathap::python {

void bindContext(py::module_& m)
{
  py::class_<numathap::Context>(m, "Context",
      R"pbdoc(
      Runtime context used when evaluating, integrating or
      differentiating a prepared expression.

      Holds symbolic definitions as strings: fixed value expressions
      (via :meth:`set_value`) and interval expressions (via
      :meth:`set_interval`). Passed alongside a :class:`_PreparedAst`
      to :func:`evaluate`, :func:`integrate` and :func:`differentiate`.
      )pbdoc")
      .def(py::init<>(),
          R"pbdoc(
          Create an empty Context with no values or intervals defined.
          )pbdoc")
      // Variable definitions
      .def(
          "set_value",
          &numathap::Context::setValue,
          py::arg("symbol"),
          py::arg("definition"),
          R"pbdoc(
          Associates a symbol with a value definition.

          Args:
              symbol: Name of the variable, e.g. "x".
              definition: Expression string defining the symbol, e.g.
                  "sqrt(2)". Not a numeric Value — a raw expression
                  that gets resolved/parsed later.
          )pbdoc")
      .def(
          "find_value",
          &numathap::Context::findValue,
          py::arg("symbol"),
          R"pbdoc(
          Returns the value definition of a symbol.

          Args:
              symbol: Name of the variable to look up.

          Returns:
              Optional[str]: The expression string previously bound via
                  :meth:`set_value`, or None if ``symbol`` is undefined.
          )pbdoc")
      .def(
          "has_value",
          &numathap::Context::hasValue,
          py::arg("symbol"),
          R"pbdoc(
          Checks whether a value definition exists.

          Args:
              symbol: Name of the variable to check.

          Returns:
              bool: True if ``symbol`` has a value bound via
                  :meth:`set_value`.
          )pbdoc")
      // Interval definitions
      .def(
          "set_interval",
          &numathap::Context::setInterval,
          py::arg("symbol"),
          py::arg("lower"),
          py::arg("upper"),
          R"pbdoc(
          Associates a symbol with an interval.

          Typically used to bind an integration variable to its
          bounds before calling :func:`integrate`.

          Args:
              symbol: Name of the variable, e.g. "x".
              lower: Expression string for the lower bound, e.g. "0".
              upper: Expression string for the upper bound, e.g. "pi/2".
          )pbdoc")
      .def(
          "find_interval",
          &numathap::Context::findInterval,
          py::arg("symbol"),
          R"pbdoc(
          Returns the interval associated with a symbol.

          Args:
              symbol: Name of the variable to look up.

          Returns:
              The interval previously bound via :meth:`set_interval`,
              exposing its lower and upper bounds as expression strings
              (e.g. "0" and "pi/2") — not numeric values. None if
              ``symbol`` has no interval defined.
              (TODO: confirm the exact Python-side shape — tuple vs.
              object with .lower()/.upper() — by inspecting
              ctx.find_interval(...) at runtime.)
          )pbdoc")
      .def(
          "has_interval",
          &numathap::Context::hasInterval,
          py::arg("symbol"),
          R"pbdoc(
          Checks whether an interval exists.

          Args:
              symbol: Name of the variable to check.

          Returns:
              bool: True if ``symbol`` has an interval bound via
                  :meth:`set_interval`.
          )pbdoc")
      // Clear
      .def(
          "clear",
          &numathap::Context::clear,
          R"pbdoc(
          Removes all stored values and intervals.
          )pbdoc");
}

}  // namespace numathap::python
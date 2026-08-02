#include <pybind11/pybind11.h>
#include <numathap/numathap.hpp>

namespace py = pybind11;

namespace numathap::python {

void bindIntegrate(py::module_& m) {
  m.def(
      "integrate",
      py::overload_cast<
          const numathap::PreparedAst&,
          const std::string&,
          const numathap::Context&
      >(&numathap::integrate),
      py::arg("expression"),
      py::arg("variable"),
      py::arg("context"),
      R"pbdoc(
      Numerically integrates a prepared expression with respect to a
      variable, using the default environment.

      The bounds of integration come from the interval bound to
      ``variable`` in ``context`` (see :meth:`Context.set_interval`).

      Args:
          expression: A previously prepared expression (see
              :func:`prepare`).
          variable: Name of the integration variable, e.g. "x". Must
              have an interval defined on ``context``.
          context: Context providing the integration interval (and any
              other variable values needed) for ``variable``.

      Returns:
          Value: The numeric result of the integral. Use ``.real()`` to
              access the underlying floating-point value.
      )pbdoc");

  m.def(
      "integrate",
      py::overload_cast<
          const numathap::PreparedAst&,
          const std::string&,
          const numathap::Context&,
          const numathap::MathEnvironment&
      >(&numathap::integrate),
      py::arg("expression"),
      py::arg("variable"),
      py::arg("context"),
      py::arg("environment"),
      R"pbdoc(
      Numerically integrates a prepared expression with respect to a
      variable, using a specific environment.

      Same as the three-argument ``integrate``, but evaluates against a
      custom :class:`MathEnvironment` instead of the default one —
      useful when the expression relies on custom functions/constants.

      Args:
          expression: A previously prepared expression (see
              :func:`prepare`).
          variable: Name of the integration variable, e.g. "x". Must
              have an interval defined on ``context``.
          context: Context providing the integration interval (and any
              other variable values needed) for ``variable``.
          environment: MathEnvironment defining the operators, functions
              and constants available during integration.

      Returns:
          Value: The numeric result of the integral. Use ``.real()`` to
              access the underlying floating-point value.
      )pbdoc");
}

}  // namespace numathap::python
#include <pybind11/pybind11.h>
#include <numathap/numathap.hpp>

#include <sstream>

namespace py = pybind11;

namespace numathap::python {

void bindPrepare(py::module_& m) {
  /*
   * Internal PreparedAst type.
   *
   * It is intentionally exposed with an underscore because it is an
   * implementation detail of the Python API.
   */
  py::class_<numathap::PreparedAst>(m, "_PreparedAst",
      R"pbdoc(
      Internal representation of an already-parsed expression (AST).

      This is returned by :func:`prepare` and consumed by functions such
      as ``evaluate``, ``integrate`` and ``differentiate``. It is an
      implementation detail — users should not need to construct it
      directly, only pass it along between calls.
      )pbdoc")
      .def("expression", &numathap::PreparedAst::expression,
          py::return_value_policy::reference_internal,
          R"pbdoc(
          Return the original expression string that produced this AST.

          Returns:
              str: The source expression, unmodified.
          )pbdoc")
      .def("empty", &numathap::PreparedAst::empty,
          R"pbdoc(
          Check whether this AST holds a parsed expression.

          Returns:
              bool: True if no expression has been prepared (empty AST).
          )pbdoc")
      .def("print", [](const numathap::PreparedAst& ast) {
          std::ostringstream os;
          ast.print(os);
          return os.str();
        },
        R"pbdoc(
        Render a human-readable, tree-like view of the parsed expression.

        Useful for debugging how an expression was parsed before
        evaluating, integrating or differentiating it.

        Returns:
            str: A textual representation of the AST.
        )pbdoc");

  /*
   * prepare(expression)
   */
  m.def(
      "prepare",
      [](const std::string& expression) {
        return numathap::prepare(expression);
      },
      py::arg("expression"),
      R"pbdoc(
      Prepare a mathematical expression using the default environment.

      Parses ``expression`` into an internal AST (:class:`_PreparedAst`)
      that can then be passed to ``evaluate``, ``integrate`` or
      ``differentiate``. Uses numathap's default :class:`MathEnvironment`
      (standard operators and functions).

      Args:
          expression: Mathematical expression as a string, e.g. "sin(x) + 2*x".

      Returns:
          _PreparedAst: The parsed expression, ready to be evaluated.
      )pbdoc");

  /*
   * prepare(expression, environment)
   */
  m.def(
      "prepare",
      [](const std::string& expression,
         const numathap::MathEnvironment& environment) {
        return numathap::prepare(expression, environment);
      },
      py::arg("expression"), py::arg("environment"),
      R"pbdoc(
      Prepare a mathematical expression using a specific environment.

      Same as ``prepare(expression)``, but parses against a custom
      :class:`MathEnvironment` instead of the default one — useful when
      you need custom functions/constants or a restricted capability set.

      Args:
          expression: Mathematical expression as a string.
          environment: MathEnvironment defining the operators, functions
              and constants available while parsing.

      Returns:
          _PreparedAst: The parsed expression, ready to be evaluated.
      )pbdoc");
}

}  // namespace numathap::python
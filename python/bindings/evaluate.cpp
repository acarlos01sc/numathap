#include <pybind11/pybind11.h>
#include <numathap/numathap.hpp>

namespace py = pybind11;

namespace numathap::python {

void bindEvaluate(py::module_& m)
{
  m.def(
      "evaluate",
      [](const numathap::PreparedAst& expression,
         const numathap::Context& context) {
        return numathap::evaluate(expression, context);
      },
      py::arg("expression"),
      py::arg("context"),
      R"pbdoc(
      Evaluate a prepared mathematical expression.

      Computes the result of ``expression`` using the values and
      settings held in ``context`` (variables, bounds, etc.). The
      expression must have gone through :func:`prepare` first.

      Args:
          expression: A previously prepared expression (see
              :func:`prepare`).
          context: Context providing variable values and any other
              runtime settings needed to evaluate the expression.

      Returns:
          Value: The result of evaluating the expression. Use
              ``.real()`` to access the underlying floating-point value.
      )pbdoc");
}

}  // namespace numathap::python
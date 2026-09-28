#include <pybind11/pybind11.h>

#include <numathap/numathap.hpp>

namespace py = pybind11;

namespace numathap::python {

void bindEvaluateCx(py::module_& m)
{
    m.def(
        "evaluate_cx",
        [](const numathap::PreparedAst& expression,
           const numathap::Context& context) {
            return numathap::evaluate_cx(expression, context);
        },
        py::arg("expression"),
        py::arg("context"),
        R"pbdoc(
Evaluate a prepared mathematical expression in the complex domain.

Computes the result of ``expression`` using the values and
settings held in ``context``. The expression must have gone
through :func:`prepare` first.

Args:
    expression: A previously prepared expression (see
        :func:`prepare`).

    context: Context providing variable values and any other
        runtime settings needed to evaluate the expression.

Returns:
    Value: The result of evaluating the expression in the complex
    domain.
)pbdoc");
}

} // namespace numathap::python
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
        "Evaluate a prepared mathematical expression.");
}

} // namespace numathap::python
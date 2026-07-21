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
    py::class_<numathap::PreparedAst>(m, "_PreparedAst")

        .def("expression", &numathap::PreparedAst::expression,
             py::return_value_policy::reference_internal)

        .def("empty", &numathap::PreparedAst::empty)

        .def("print", [](const numathap::PreparedAst& ast) {
            std::ostringstream os;
            ast.print(os);
            return os.str();
        });

    /*
     * prepare(expression)
     */
    m.def(
        "prepare",
        [](const std::string& expression) {
            return numathap::prepare(expression);
        },
        py::arg("expression"),
        "Prepare a mathematical expression using the default environment.");

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
        "Prepare a mathematical expression using a specific environment.");
}

}  // namespace numathap::python
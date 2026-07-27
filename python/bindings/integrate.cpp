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
        py::arg("context"));

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
        py::arg("environment"));
}

} // namespace numathap::python
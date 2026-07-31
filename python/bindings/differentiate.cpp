#include <pybind11/pybind11.h>

#include <numathap/numathap.hpp>

namespace py = pybind11;

namespace numathap::python {

void bindDifferentiate(py::module_& m) {

    m.def(
        "differentiate",
        py::overload_cast<
            const numathap::PreparedAst&,
            const std::string&
        >(&numathap::differentiate),
        py::arg("expression"),
        py::arg("variable"));
}

} // namespace numathap::python
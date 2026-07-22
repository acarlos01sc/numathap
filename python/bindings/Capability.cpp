#include "numathap/config/Capability.hpp"

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace numathap::python {

void bindCapability(py::module_& m) {
    py::enum_<numathap::config::Capability>(
        m, "Capability", "Available processing capabilities.")
        .value("Simplify", numathap::config::Capability::Simplify,
               "Enables symbolic simplification of the Math-AST.")
        .export_values();
}

} // numathap::python
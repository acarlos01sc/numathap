#include "numathap/config/Capability.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace numathap::python {

void bindCapability(py::module_& m) {

    py::enum_<numathap::config::Capability>(
        m, "Capability",

        R"pbdoc(
Available processing capabilities.

Pass to :func:`configure` to build a MathEnvironment with the
given capability enabled.
)pbdoc")

        .value("ConstantFolder",
               numathap::config::Capability::ConstantFolder,

               R"pbdoc(
Enables constant folding of the Math-AST.
)pbdoc")

        .value("Simplify",
               numathap::config::Capability::Simplify,

               R"pbdoc(
Enables symbolic simplification of the Math-AST.
)pbdoc")

        .value("UltraSimplifier",
               numathap::config::Capability::UltraSimplifier,

               R"pbdoc(
Enables advanced symbolic simplification of the Math-AST.
)pbdoc")

        .export_values();
}

} // namespace numathap::python
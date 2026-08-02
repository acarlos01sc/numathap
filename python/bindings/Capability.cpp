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
      .value("Simplify", numathap::config::Capability::Simplify,
          R"pbdoc(
          Enables symbolic simplification of the Math-AST.
          )pbdoc")
      .export_values();
}

}  // numathap::python
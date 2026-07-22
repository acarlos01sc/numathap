#include <pybind11/pybind11.h>

#include "numathap/config/configure.hpp"
#include "numathap/config/Capability.hpp"
#include "numathap/config/MathEnvironment.hpp"

namespace py = pybind11;

namespace numathap::python {

void bindConfigure(py::module_& m)
{
    m.def(
        "configure",
        []() {
            return numathap::config::configure();
        },
        "Creates a default MathEnvironment.");

    m.def(
        "configure",
        [](numathap::config::Capability capability) {
            return numathap::config::configure(capability);
        },
        py::arg("capability"),
        "Creates a MathEnvironment with the specified capability enabled.");
}

} // numathap::python
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <numathap/numathap.hpp>

namespace py = pybind11;

namespace numathap::python {

void bindContext(py::module_& m)
{
    py::class_<numathap::Context>(m, "Context")

        .def(py::init<>())

        // Variable definitions
        .def(
            "set_value",
            &numathap::Context::setValue,
            py::arg("symbol"),
            py::arg("definition"),
            "Associates a symbol with a value definition.")

        .def(
            "find_value",
            &numathap::Context::findValue,
            py::arg("symbol"),
            "Returns the value definition of a symbol.")

        .def(
            "has_value",
            &numathap::Context::hasValue,
            py::arg("symbol"),
            "Checks whether a value definition exists.")

        // Interval definitions
        .def(
            "set_interval",
            &numathap::Context::setInterval,
            py::arg("symbol"),
            py::arg("lower"),
            py::arg("upper"),
            "Associates a symbol with an interval.")

        .def(
            "find_interval",
            &numathap::Context::findInterval,
            py::arg("symbol"),
            "Returns the interval associated with a symbol.")

        .def(
            "has_interval",
            &numathap::Context::hasInterval,
            py::arg("symbol"),
            "Checks whether an interval exists.")

        // Clear
        .def(
            "clear",
            &numathap::Context::clear,
            "Removes all stored values and intervals.");
}

} // namespace numathap::python
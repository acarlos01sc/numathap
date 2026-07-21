#include <pybind11/pybind11.h>

#include <numathap/numathap.hpp>
#include <sstream>

namespace py = pybind11;

namespace numathap::python {

void bindValue(py::module_& m) {
    py::class_<numathap::Value>(m, "Value")

        .def(py::init<>())

        .def(py::init<double>())

        .def("__add__", [](const numathap::Value& a,
                           const numathap::Value& b) { return a + b; })

        .def("__sub__", [](const numathap::Value& a,
                           const numathap::Value& b) { return a - b; })

        .def("__mul__", [](const numathap::Value& a,
                           const numathap::Value& b) { return a * b; })

        .def("__truediv__", [](const numathap::Value& a,
                               const numathap::Value& b) { return a / b; })

        .def("__neg__", [](const numathap::Value& v) { return -v; })

        .def("__pos__", [](const numathap::Value& v) { return +v; })

        .def("__eq__", [](const numathap::Value& a,
                          const numathap::Value& b) { return a == b; })

        .def("__ne__", [](const numathap::Value& a,
                          const numathap::Value& b) { return a != b; })

        .def("__repr__",
             [](const numathap::Value& value) {
                 std::ostringstream os;
                 os << value;
                 return os.str();
             })

        .def("real", &numathap::Value::real,
             py::return_value_policy::reference_internal,
             "Access the underlying real value.");
}

}  // namespace numathap::python
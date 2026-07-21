#include <pybind11/pybind11.h>

#include <numathap/numathap.hpp>

namespace py = pybind11;

namespace numathap::python {

void bindMathEnvironment(py::module_& m) {
    // MathLibrary
    py::enum_<numathap::MathLibrary>(m, "MathLibrary")
        .value("CMath", numathap::MathLibrary::CMath)
        .export_values();

    // NumericType
    py::enum_<numathap::NumericType>(m, "NumericType")
        .value("Double", numathap::NumericType::Double)
        .export_values();

    // MathEnvironment
    py::class_<numathap::MathEnvironment>(m, "MathEnvironment")
        .def(py::init<>())

        .def_property_readonly("math_library",
                               &numathap::MathEnvironment::mathLibrary,
                               "Active mathematical library.")

        .def_property_readonly("numeric_type",
                               &numathap::MathEnvironment::numericType,
                               "Active numeric type.");
}

}  // namespace numathap::python
#include <pybind11/pybind11.h>
#include <numathap/numathap.hpp>

#include <sstream>

namespace py = pybind11;

namespace numathap::python {

void bindValue(py::module_& m) {
  py::class_<numathap::Value>(m, "Value",
      R"pbdoc(
      Numeric result type used throughout numathap.

      Returned by :func:`evaluate` and used internally wherever a
      computed number is produced. Supports the standard arithmetic
      (``+ - * / - +``) and comparison (``== != < <= > >=``) operators,
      so it can usually be used like a plain number. Use :meth:`real`
      to get the underlying floating-point value.
      )pbdoc")
      .def(py::init<>(),
          R"pbdoc(
          Create a Value initialized to zero.
          )pbdoc")
      .def(py::init<double>(),
          R"pbdoc(
          Create a Value from a Python float.

          Args:
              value: The floating-point number to wrap.
          )pbdoc")
      // Arithmetic operators
      .def("__add__",
          [](const numathap::Value& a, const numathap::Value& b) {
            return a + b;
          },
          R"pbdoc(
          Add two Values.

          Returns:
              Value: The sum ``self + other``.
          )pbdoc")
      .def("__sub__",
          [](const numathap::Value& a, const numathap::Value& b) {
            return a - b;
          },
          R"pbdoc(
          Subtract two Values.

          Returns:
              Value: The difference ``self - other``.
          )pbdoc")
      .def("__mul__",
          [](const numathap::Value& a, const numathap::Value& b) {
            return a * b;
          },
          R"pbdoc(
          Multiply two Values.

          Returns:
              Value: The product ``self * other``.
          )pbdoc")
      .def("__truediv__",
          [](const numathap::Value& a, const numathap::Value& b) {
            return a / b;
          },
          R"pbdoc(
          Divide two Values.

          Returns:
              Value: The quotient ``self / other``.
          )pbdoc")
      .def("__neg__", [](const numathap::Value& v) { return -v; },
          R"pbdoc(
          Return the negation of this Value.

          Returns:
              Value: ``-self``.
          )pbdoc")
      .def("__pos__", [](const numathap::Value& v) { return +v; },
          R"pbdoc(
          Return this Value unchanged (unary plus).

          Returns:
              Value: ``+self``.
          )pbdoc")
      // Comparison operators
      .def("__eq__",
          [](const numathap::Value& a, const numathap::Value& b) {
            return a == b;
          },
          R"pbdoc(
          Check equality between two Values.

          Returns:
              bool: True if ``self == other``.
          )pbdoc")
      .def("__ne__",
          [](const numathap::Value& a, const numathap::Value& b) {
            return a != b;
          },
          R"pbdoc(
          Check inequality between two Values.

          Returns:
              bool: True if ``self != other``.
          )pbdoc")
      .def("__lt__",
          [](const numathap::Value& a, const numathap::Value& b) {
            return a < b;
          },
          R"pbdoc(
          Check if this Value is less than another.

          Returns:
              bool: True if ``self < other``.
          )pbdoc")
      .def("__le__",
          [](const numathap::Value& a, const numathap::Value& b) {
            return a <= b;
          },
          R"pbdoc(
          Check if this Value is less than or equal to another.

          Returns:
              bool: True if ``self <= other``.
          )pbdoc")
      .def("__gt__",
          [](const numathap::Value& a, const numathap::Value& b) {
            return a > b;
          },
          R"pbdoc(
          Check if this Value is greater than another.

          Returns:
              bool: True if ``self > other``.
          )pbdoc")
      .def("__ge__",
          [](const numathap::Value& a, const numathap::Value& b) {
            return a >= b;
          },
          R"pbdoc(
          Check if this Value is greater than or equal to another.

          Returns:
              bool: True if ``self >= other``.
          )pbdoc")
      .def("__repr__",
          [](const numathap::Value& value) {
            std::ostringstream os;
            os << value;
            return os.str();
          },
          R"pbdoc(
          Return a readable string representation of this Value.

          Returns:
              str: The formatted value, as used by ``repr()``.
          )pbdoc")
      .def("real", &numathap::Value::real,
          py::return_value_policy::reference_internal,
          R"pbdoc(
          Access the underlying real value.

          Returns:
              float: The plain floating-point number wrapped by this
                  Value.
          )pbdoc");
}

}  // namespace numathap::python
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
      py::arg("variable"),
      R"pbdoc(
      Symbolically differentiates a prepared expression with respect to
      a variable.

      Args:
          expression: A previously prepared expression (see
              :func:`prepare`).
          variable: Name of the variable to differentiate with respect
              to, e.g. "x".

      Returns:
          A new expression representing the derivative. (TODO: confirm
          the exact Python type of the result — likely another
          :class:`_PreparedAst`, but this file only has one overload
          and no MathEnvironment-accepting variant, unlike
          :func:`integrate`; worth double-checking against
          ``numathap.hpp``.)
      )pbdoc");
}

}  // namespace numathap::python
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace numathap::python {

void bindMathEnvironment(py::module_& m);
void bindValue(py::module_& m);
void bindContext(py::module_& m);
void bindPrepare(py::module_& m);
void bindEvaluate(py::module_& m);
void bindCapability(py::module_& m);
void bindConfigure(py::module_& m);
// Integration
void bindAlgorithm(py::module_& m);
void bindIntegrate(py::module_& m);

}  // namespace numathap::python

PYBIND11_MODULE(_numathap, m) {
    // Core
    numathap::python::bindValue(m);
    numathap::python::bindContext(m);

    // Configuration
    numathap::python::bindMathEnvironment(m);
    numathap::python::bindCapability(m);
    numathap::python::bindConfigure(m);

    // Expression processing
    numathap::python::bindPrepare(m);
    numathap::python::bindEvaluate(m);

    // Numerical integration
    numathap::python::bindAlgorithm(m);
    numathap::python::bindIntegrate(m);
}
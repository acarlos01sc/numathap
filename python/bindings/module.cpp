#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace numathap::python {

void bindMathEnvironment(py::module_& m);
void bindValue(py::module_& m);
void bindContext(py::module_& m);
void bindPrepare(py::module_& m);
void bindEvaluate(py::module_& m);

}

PYBIND11_MODULE(_numathap, m)
{
    numathap::python::bindMathEnvironment(m);
    numathap::python::bindValue(m);
    numathap::python::bindContext(m);
    numathap::python::bindPrepare(m);
    numathap::python::bindEvaluate(m);
}
#include <pybind11/pybind11.h>

#include <numathap/numathap.hpp>

namespace py = pybind11;

namespace numathap::python {

void bindAlgorithm(py::module_& m) {
    py::enum_<numathap::backend::integrate::Algorithm>(m, "Algorithm")
        .value("AdaptiveSimpson",
               numathap::backend::integrate::Algorithm::AdaptiveSimpson)
        .value("GaussKronrod15",
               numathap::backend::integrate::Algorithm::GaussKronrod15)
        .export_values();
}

}  // namespace numathap::python
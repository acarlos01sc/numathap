#include "numathap/config/configure.hpp"

#include <pybind11/pybind11.h>

#include <stdexcept>

#include "numathap/backend/integrate/AdaptiveSimpson.hpp"
#include "numathap/backend/integrate/Algorithm.hpp"
#include "numathap/backend/integrate/GaussKronrod15.hpp"
#include "numathap/config/Capability.hpp"
#include "numathap/config/MathEnvironment.hpp"

namespace py = pybind11;

namespace numathap::python {

void bindConfigure(py::module_& m) {
    // Adaptive Simpson configuration
    py::class_<numathap::backend::integrate::AdaptiveSimpsonConfig>(
        m, "AdaptiveSimpsonConfig")
        .def(py::init<>())
        .def_readwrite(
            "tolerance",
            &numathap::backend::integrate::AdaptiveSimpsonConfig::tolerance)
        .def_readwrite(
            "maxDepth",
            &numathap::backend::integrate::AdaptiveSimpsonConfig::maxDepth);

    // Gauss-Kronrod 15 configuration
    py::class_<numathap::backend::integrate::GaussKronrod15Config>(
        m, "GaussKronrod15Config")
        .def(py::init<>())
        .def_readwrite(
            "absoluteTolerance",
            &numathap::backend::integrate::GaussKronrod15Config::
                absoluteTolerance)
        .def_readwrite(
            "relativeTolerance",
            &numathap::backend::integrate::GaussKronrod15Config::
                relativeTolerance)
        .def_readwrite(
            "maxEvaluations",
            &numathap::backend::integrate::GaussKronrod15Config::
                maxEvaluations);

    // Default configuration
    m.def(
        "configure", []() { return numathap::config::configure(); },
        "Creates a default MathEnvironment.");

    // Configuration with a capability
    m.def(
        "configure",
        [](numathap::config::Capability capability) {
            return numathap::config::configure(capability);
        },
        py::arg("capability"),
        "Creates a MathEnvironment with the specified capability enabled.");

    // Configuration with an integration algorithm
    m.def(
        "configure",
        [](numathap::backend::integrate::Algorithm algorithm) {
            using namespace numathap::backend::integrate;

            switch (algorithm) {
                case Algorithm::AdaptiveSimpson:
                    return numathap::config::configure(
                        AdaptiveSimpsonConfig{});

                case Algorithm::GaussKronrod15:
                    return numathap::config::configure(GaussKronrod15Config{});
            }

            throw std::runtime_error("Unknown integration algorithm.");
        },
        py::arg("algorithm"),
        "Creates a MathEnvironment using the specified integration "
        "algorithm with its default configuration.");

    // Configuration with Adaptive Simpson parameters
    m.def(
        "configure",
        [](const numathap::backend::integrate::AdaptiveSimpsonConfig& config) {
            return numathap::config::configure(config);
        },
        py::arg("config"),
        "Creates a MathEnvironment using the specified Adaptive Simpson "
        "configuration.");

    // Configuration with Gauss-Kronrod 15 parameters
    m.def(
        "configure",
        [](const numathap::backend::integrate::GaussKronrod15Config& config) {
            return numathap::config::configure(config);
        },
        py::arg("config"),
        "Creates a MathEnvironment using the specified Gauss-Kronrod 15 "
        "configuration.");
}

}  // namespace numathap::python
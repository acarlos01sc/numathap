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
                    return numathap::config::configure(AdaptiveSimpsonConfig{});

                case Algorithm::GaussKronrod15:
                    return numathap::config::configure(GaussKronrod15Config{});
            }

            throw std::runtime_error("Unknown integration algorithm.");
        },
        py::arg("algorithm"),
        "Creates a MathEnvironment using the specified integration "
        "algorithm with its default configuration.");
}

}  // namespace numathap::python
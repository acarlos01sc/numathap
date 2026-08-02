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
      m, "AdaptiveSimpsonConfig",
      R"pbdoc(
      Tuning parameters for the Adaptive Simpson integration algorithm.

      Pass an instance to :func:`configure` to build a MathEnvironment
      that integrates using these settings.
      )pbdoc")
      .def(py::init<>(),
          R"pbdoc(
          Create an AdaptiveSimpsonConfig with default tolerance and
          max depth.
          )pbdoc")
      .def_readwrite(
          "tolerance",
          &numathap::backend::integrate::AdaptiveSimpsonConfig::tolerance,
          R"pbdoc(
          float: Target error tolerance for the adaptive subdivision.
          Smaller values mean more accuracy at the cost of more
          function evaluations.
          )pbdoc")
      .def_readwrite(
          "maxDepth",
          &numathap::backend::integrate::AdaptiveSimpsonConfig::maxDepth,
          R"pbdoc(
          int: Maximum recursion depth for interval subdivision, used
          as a safety limit against non-convergence.
          )pbdoc");

  // Gauss-Kronrod 15 configuration
  py::class_<numathap::backend::integrate::GaussKronrod15Config>(
      m, "GaussKronrod15Config",
      R"pbdoc(
      Tuning parameters for the Gauss-Kronrod 15-point integration
      algorithm.

      Pass an instance to :func:`configure` to build a MathEnvironment
      that integrates using these settings.
      )pbdoc")
      .def(py::init<>(),
          R"pbdoc(
          Create a GaussKronrod15Config with default tolerances and
          evaluation limit.
          )pbdoc")
      .def_readwrite(
          "absoluteTolerance",
          &numathap::backend::integrate::GaussKronrod15Config::
              absoluteTolerance,
          R"pbdoc(
          float: Maximum acceptable absolute error of the estimate.
          )pbdoc")
      .def_readwrite(
          "relativeTolerance",
          &numathap::backend::integrate::GaussKronrod15Config::
              relativeTolerance,
          R"pbdoc(
          float: Maximum acceptable relative error of the estimate.
          )pbdoc")
      .def_readwrite(
          "maxEvaluations",
          &numathap::backend::integrate::GaussKronrod15Config::
              maxEvaluations,
          R"pbdoc(
          int: Upper bound on function evaluations, used as a safety
          limit against non-convergence.
          )pbdoc");

  // Default configuration
  m.def(
      "configure", []() { return numathap::config::configure(); },
      R"pbdoc(
      Creates a default MathEnvironment.

      Returns:
          MathEnvironment: An environment with default capabilities and
              default integration settings.
      )pbdoc");

  // Configuration with a capability
  m.def(
      "configure",
      [](numathap::config::Capability capability) {
        return numathap::config::configure(capability);
      },
      py::arg("capability"),
      R"pbdoc(
      Creates a MathEnvironment with the specified capability enabled.

      Args:
          capability: The capability to enable on the resulting
              environment.

      Returns:
          MathEnvironment: A new environment with ``capability``
              enabled.
      )pbdoc");

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
      R"pbdoc(
      Creates a MathEnvironment using the specified integration
      algorithm with its default configuration.

      Shortcut for picking an algorithm without tuning its parameters
      — equivalent to ``configure(AdaptiveSimpsonConfig())`` or
      ``configure(GaussKronrod15Config())`` depending on ``algorithm``.

      Args:
          algorithm: Which integration algorithm to use (see
              :class:`Algorithm`).

      Returns:
          MathEnvironment: An environment configured to integrate with
              ``algorithm`` using its default settings.
      )pbdoc");

  // Configuration with Adaptive Simpson parameters
  m.def(
      "configure",
      [](const numathap::backend::integrate::AdaptiveSimpsonConfig& config) {
        return numathap::config::configure(config);
      },
      py::arg("config"),
      R"pbdoc(
      Creates a MathEnvironment using the specified Adaptive Simpson
      configuration.

      Args:
          config: Tolerance/max-depth settings for the Adaptive Simpson
              algorithm.

      Returns:
          MathEnvironment: An environment configured to integrate with
              Adaptive Simpson using ``config``.
      )pbdoc");

  // Configuration with Gauss-Kronrod 15 parameters
  m.def(
      "configure",
      [](const numathap::backend::integrate::GaussKronrod15Config& config) {
        return numathap::config::configure(config);
      },
      py::arg("config"),
      R"pbdoc(
      Creates a MathEnvironment using the specified Gauss-Kronrod 15
      configuration.

      Args:
          config: Tolerance/max-evaluations settings for the
              Gauss-Kronrod 15 algorithm.

      Returns:
          MathEnvironment: An environment configured to integrate with
              Gauss-Kronrod 15 using ``config``.
      )pbdoc");
}

}  // namespace numathap::python
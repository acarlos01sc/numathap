/**
 * @file AdaptiveSimpson.hpp
 * @brief Defines the configuration and implementation interface for the
 *        Adaptive Simpson integration algorithm.
 */

#pragma once

#include <cstddef>
#include <string>

#include "numathap/core/Value.hpp"

namespace numathap::math {
class PreparedAst;
}

namespace numathap::backend::integrate {

/**
 * @brief Configuration parameters for the Adaptive Simpson integration
 * algorithm.
 *
 * Controls the precision and recursion limits for the adaptive quadrature
 * process.
 */
struct AdaptiveSimpsonConfig {
    core::Value tolerance = core::Value(1e-10);
    std::size_t maxDepth = 20;
};

/**
 * @brief Implements the Adaptive Simpson's rule for numerical integration.
 *
 * This class provides a static interface to compute the definite integral
 * of a given @ref math::PreparedAst over a specified interval [lower, upper].
 * The algorithm adaptively subdivides the interval until the estimated error
 * falls below the provided @ref AdaptiveSimpsonConfig::tolerance.
 */
class AdaptiveSimpson {
   public:
    /**
     * @brief Integrates a prepared mathematical expression.
     *
     * @param prepared Prepared mathematical expression.
     * @param variable Name of the integration variable.
     * @param lower Lower integration limit.
     * @param upper Upper integration limit.
     * @param config Adaptive Simpson configuration.
     * @return Numerical value of the integral.
     *
     * @throws std::invalid_argument if the tolerance is not positive.
     * @throws std::invalid_argument if maxDepth is zero.
     */
    [[nodiscard]]
    static core::Value integrate(const math::PreparedAst& prepared,
                                 const std::string& variable,
                                 const core::Value& lower,
                                 const core::Value& upper,
                                 const AdaptiveSimpsonConfig& config);
};

}  // namespace numathap::backend::integrate
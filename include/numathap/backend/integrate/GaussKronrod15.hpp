/**
 * @file GaussKronrod15.hpp
 * @brief Defines the configuration and implementation interface for the
 *        adaptive Gauss-Kronrod 15-point integration algorithm.
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
 * @brief Configuration parameters for the Gauss-Kronrod 15-point integration
 * algorithm.
 *
 * Controls the error estimation criteria and the adaptive refinement process.
 *
 * The algorithm combines a 7-point Gauss rule and a 15-point Kronrod rule.
 * The difference between both approximations provides an estimate of the
 * local integration error.
 */
struct GaussKronrod15Config {
    /**
     * @brief Absolute error tolerance.
     *
     * The estimated error is accepted when it is below this threshold
     * for integrals whose magnitude is close to zero.
     */
    core::Value absoluteTolerance = core::Value(1e-10);

    /**
     * @brief Relative error tolerance.
     *
     * Controls the required accuracy relative to the magnitude of the
     * computed integral.
     */
    core::Value relativeTolerance = core::Value(1e-10);

    /**
     * @brief Maximum number of function evaluations.
     *
     * Prevents excessive refinement when the requested tolerance cannot
     * be achieved.
     */
    std::size_t maxEvaluations = 10000;
};

/**
 * @brief Implements the adaptive Gauss-Kronrod 15-point quadrature rule.
 *
 * This class provides a static interface to compute the definite integral
 * of a given @ref math::PreparedAst over a specified interval.
 *
 * The algorithm evaluates the embedded 7-point Gauss and 15-point Kronrod
 * rules using a shared set of function evaluations. The difference between
 * both results is used as a local error estimate to guide adaptive interval
 * subdivision.
 */
class GaussKronrod15 {
   public:
    /**
     * @brief Computes the definite integral of a prepared expression.
     *
     * @param prepared Prepared mathematical expression.
     * @param variable Name of the integration variable.
     * @param lower Lower integration limit.
     * @param upper Upper integration limit.
     * @param config Gauss-Kronrod 15-point configuration.
     *
     * @return Numerical approximation of the integral.
     *
     * @throws std::invalid_argument if tolerances are not positive.
     * @throws std::invalid_argument if maxEvaluations is zero.
     */
    [[nodiscard]]
    static core::Value integrate(const math::PreparedAst& prepared,
                                 const std::string& variable,
                                 const core::Value& lower,
                                 const core::Value& upper,
                                 const GaussKronrod15Config& config);
};

}  // namespace numathap::backend::integrate
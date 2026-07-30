/**
 * @file Integrator.hpp
 * @brief Defines the integration dispatcher used by the numeric backend.
 */

#pragma once

#include <string>

#include "numathap/config/MathEnvironment.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/math/PreparedAst.hpp"

namespace numathap::backend::integrate {

/**
 * @brief Numerical integration dispatcher.
 *
 * The @c Integrator acts as a central hub that analyzes the integration bounds
 * and dispatches the task to the appropriate numerical integration strategy
 * selected by the @ref config::MathEnvironment.
 *
 * Current functionality includes:
 * - Detecting finite vs. improper intervals.
 * - Selecting the configured integration algorithm.
 * - Dispatching the task to the selected numerical integration routine.
 *
 * @note This class encapsulates the integration strategy selection logic;
 *       adding support for new integration algorithms should be handled
 *       through the configured @ref integrate::Algorithm.
 */
class Integrator {
   public:
    /**
     * @brief Integrates an expression using the default environment settings.
     */
    core::Value integrate(const math::PreparedAst& prepared,
                          const std::string& variable,
                          const core::Context& context) const;

    /**
     * @brief Integrates an expression using a specific @ref
     * config::MathEnvironment.
     */
    core::Value integrate(const math::PreparedAst& prepared,
                          const std::string& variable,
                          const core::Context& context,
                          const config::MathEnvironment& environment) const;

   private:
    /** @brief Internal representation of integration bounds retrieved from
     * context. */
    struct IntegrationInterval {
        std::string lower;
        std::string upper;
    };

    /** @brief Resolves interval bounds from the provided context. */
    IntegrationInterval resolveInterval(const std::string& variable,
                                        const core::Context& context) const;

    /** @brief Checks if a given integration bound represents an infinite value.
     */
    bool isInfinite(const std::string& bound) const;

    /**
     * @brief Executes the finite-interval integration workflow.
     *
     * The configured integration algorithm is selected through the provided
     * mathematical environment.
     */
    core::Value integrateFinite(
        const math::PreparedAst& prepared, const std::string& variable,
        const IntegrationInterval& interval,
        const config::MathEnvironment& environment) const;

    /**
     * @brief Executes the improper-interval integration workflow.
     *
     * The configured integration algorithm is selected through the provided
     * mathematical environment.
     */
    core::Value integrateImproper(
        const math::PreparedAst& prepared, const std::string& variable,
        const IntegrationInterval& interval,
        const config::MathEnvironment& environment) const;
};

}  // namespace numathap::backend::integrate
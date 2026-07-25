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
 * @brief Numeric integration dispatcher.
 *
 * This class is responsible for selecting the appropriate integration
 * workflow according to the integration interval.
 *
 * Current behavior:
 * - Finite intervals are forwarded to the finite integration stub.
 * - Infinite intervals are forwarded to the improper integration stub.
 *
 * Future versions will implement:
 * - Variable transformations for improper integrals.
 * - Dispatch to the selected numerical integration algorithm.
 */
class Integrator {
   public:
    /**
     * @brief Integrates an expression using the default math environment.
     *
     * @param prepared Prepared expression.
     * @param variable Integration variable.
     * @param context Evaluation context containing the integration interval.
     * @return Computed integral.
     */
    core::Value integrate(const math::PreparedAst& prepared,
                          const std::string& variable,
                          const core::Context& context) const;

    /**
     * @brief Integrates an expression using the supplied math environment.
     *
     * @param prepared Prepared expression.
     * @param variable Integration variable.
     * @param context Evaluation context containing the integration interval.
     * @param environment Math environment.
     * @return Computed integral.
     */
    core::Value integrate(const math::PreparedAst& prepared,
                          const std::string& variable,
                          const core::Context& context,
                          const config::MathEnvironment& environment) const;

   private:
    struct IntegrationInterval {
        core::Value lower;
        core::Value upper;

        bool lowerInfinite = false;
        bool upperInfinite = false;
    };

    IntegrationInterval resolveInterval(const std::string& variable,
                                        const core::Context& context) const;

    core::Value integrateFinite(
        const math::PreparedAst& prepared, const std::string& variable,
        const IntegrationInterval& interval,
        const config::MathEnvironment& environment) const;

    core::Value integrateImproper(
        const math::PreparedAst& prepared, const std::string& variable,
        const IntegrationInterval& interval,
        const config::MathEnvironment& environment) const;
};

}  // namespace numathap::backend::integrate
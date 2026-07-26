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
 * This class selects the appropriate integration workflow according to
 * the integration interval.
 *
 * Current behavior:
 * - Finite intervals are forwarded to the finite integration stub.
 * - Improper intervals are forwarded to the improper integration stub.
 *
 * Future versions will:
 * - Evaluate symbolic finite limits.
 * - Transform improper integrals using the tangent substitution.
 * - Dispatch to the selected numerical integration algorithm.
 */
class Integrator {
   public:
    core::Value integrate(const math::PreparedAst& prepared,
                          const std::string& variable,
                          const core::Context& context) const;

    core::Value integrate(const math::PreparedAst& prepared,
                          const std::string& variable,
                          const core::Context& context,
                          const config::MathEnvironment& environment) const;

   private:
    struct IntegrationInterval {
        std::string lower;
        std::string upper;
    };

    IntegrationInterval resolveInterval(const std::string& variable,
                                        const core::Context& context) const;

    bool isInfinite(const std::string& bound) const;

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
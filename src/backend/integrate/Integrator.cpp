/**
 * @file Integrator.cpp
 * @brief Implements the integration dispatcher.
 */

#include "numathap/backend/integrate/Integrator.hpp"

#include <stdexcept>

namespace numathap::backend::integrate {

using config::MathEnvironment;
using core::Context;
using core::Value;
using math::PreparedAst;

Value Integrator::integrate(const PreparedAst& prepared,
                            const std::string& variable,
                            const Context& context) const {
    return integrate(prepared, variable, context, MathEnvironment{});
}

Value Integrator::integrate(const PreparedAst& prepared,
                            const std::string& variable, const Context& context,
                            const MathEnvironment& environment) const {
    IntegrationInterval interval = resolveInterval(variable, context);

    if (isInfinite(interval.lower) || isInfinite(interval.upper)) {
        return integrateImproper(prepared, variable, interval, environment);
    }

    return integrateFinite(prepared, variable, interval, environment);
}

Integrator::IntegrationInterval Integrator::resolveInterval(
    const std::string& variable, const Context& context) const {
    auto interval = context.findInterval(variable);

    if (!interval) {
        throw std::runtime_error(
            "Integration interval not found for variable '" + variable + "'.");
    }

    return IntegrationInterval{context.resolveValue(interval->lower),
                               context.resolveValue(interval->upper)};
}

bool Integrator::isInfinite(const std::string& bound) const {
    return bound == "inf" || bound == "+inf" || bound == "-inf";
}

Value Integrator::integrateFinite(const PreparedAst& prepared,
                                  const std::string& variable,
                                  const IntegrationInterval& interval,
                                  const MathEnvironment& environment) const {
    (void)prepared;
    (void)variable;
    (void)interval;
    (void)environment;

    //
    // Future implementation:
    //
    // 1. Evaluate interval.lower -> Value
    // 2. Evaluate interval.upper -> Value
    // 3. Dispatch to the selected numerical algorithm
    //

    throw std::runtime_error("Finite interval integration not implemented.");
}

Value Integrator::integrateImproper(const PreparedAst& prepared,
                                    const std::string& variable,
                                    const IntegrationInterval& interval,
                                    const MathEnvironment& environment) const {
    (void)prepared;
    (void)variable;
    (void)interval;
    (void)environment;

    //
    // Future implementation:
    //
    // 1. Detect the type of improper interval.
    // 2. Apply the tangent variable transformation.
    // 3. Produce an equivalent finite interval.
    // 4. Evaluate the transformed limits.
    // 5. Dispatch to the selected numerical algorithm.
    //

    throw std::runtime_error("Improper interval integration not implemented.");
}

}  // namespace numathap::backend::integrate
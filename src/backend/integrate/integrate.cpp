/**
 * @file integrate.cpp
 * @brief Implements the public numerical integration entry point.
 */

#include "numathap/backend/integrate/integrate.hpp"

#include "numathap/backend/integrate/Integrator.hpp"

namespace numathap::backend::integrate {

core::Value integrate(
    const math::PreparedAst& expression,
    const std::string& variable,
    const core::Context& context) {
    return backend::integrate::Integrator{}.integrate(
        expression,
        variable,
        context);
}

core::Value integrate(
    const math::PreparedAst& expression,
    const std::string& variable,
    const core::Context& context,
    const config::MathEnvironment& environment) {
    return backend::integrate::Integrator{}.integrate(
        expression,
        variable,
        context,
        environment);
}

}  // namespace numathap::backend::integrate
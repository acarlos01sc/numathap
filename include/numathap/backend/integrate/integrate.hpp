/**
 * @file integrate.hpp
 * @brief Provides the public entry point for numerical integration.
 */

#pragma once

#include <string>

#include "numathap/config/MathEnvironment.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/math/PreparedAst.hpp"

namespace numathap::backend::integrate {

/**
 * @brief Integrates a prepared mathematical expression over an interval.
 *
 * The integration interval is obtained from the supplied Context using
 * the specified variable. The default MathEnvironment selects the
 * default integration algorithm and its default configuration.
 *
 * @param expression Prepared mathematical expression.
 * @param variable Name of the integration variable.
 * @param context Context containing the integration interval.
 * @return Numerical value of the integral.
 */
[[nodiscard]]
core::Value integrate(
    const math::PreparedAst& expression,
    const std::string& variable,
    const core::Context& context);

/**
 * @brief Integrates a prepared mathematical expression using a specific
 *        MathEnvironment.
 *
 * The MathEnvironment determines the integration algorithm and its
 * configuration.
 *
 * @param expression Prepared mathematical expression.
 * @param variable Name of the integration variable.
 * @param context Context containing the integration interval.
 * @param environment Configuration of the integration environment.
 * @return Numerical value of the integral.
 */
[[nodiscard]]
core::Value integrate(
    const math::PreparedAst& expression,
    const std::string& variable,
    const core::Context& context,
    const config::MathEnvironment& environment);

}  // namespace numathap::backend::integrate
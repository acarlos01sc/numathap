/**
 * @file series.hpp
 * @brief Provides the public entry point for series generation.
 */

#pragma once

#include <string>

#include "numathap/core/Context.hpp"
#include "numathap/math/PreparedAst.hpp"

namespace numathap::config {
class MathEnvironment;
}

namespace numathap::backend::series {

/**
 * @brief Builds a series expansion using the default environment.
 *
 * This overload creates a default @ref config::MathEnvironment and uses
 * the default series configuration.
 *
 * @param prepared Prepared mathematical expression.
 * @param variable Name of the expansion variable.
 * @param center Expansion center expression.
 *
 * @return A @ref math::PreparedAst containing the generated series.
 */
[[nodiscard]]
math::PreparedAst series(const math::PreparedAst& prepared,
                         const std::string& variable,
                         const std::string& center);

/**
 * @brief Builds a series expansion using a context.
 *
 * The context is used to resolve symbols required to evaluate the expansion
 * center or other values involved during series generation.
 *
 * @param prepared Prepared mathematical expression.
 * @param variable Name of the expansion variable.
 * @param center Expansion center expression.
 * @param context Context containing symbol values.
 *
 * @return A @ref math::PreparedAst containing the generated series.
 */
[[nodiscard]]
math::PreparedAst series(const math::PreparedAst& prepared,
                         const std::string& variable,
                         const std::string& center,
                         const core::Context& context);

/**
 * @brief Builds a series expansion using a mathematical environment.
 *
 * The environment defines the active series backend and its configuration.
 *
 * @param prepared Prepared mathematical expression.
 * @param variable Name of the expansion variable.
 * @param center Expansion center expression.
 * @param environment Mathematical environment containing series settings.
 *
 * @return A @ref math::PreparedAst containing the generated series.
 */
[[nodiscard]]
math::PreparedAst series(const math::PreparedAst& prepared,
                         const std::string& variable,
                         const std::string& center,
                         const config::MathEnvironment& environment);

/**
 * @brief Builds a series expansion.
 *
 * This overload provides full control over symbol resolution and series
 * configuration.
 *
 * @param prepared Prepared mathematical expression.
 * @param variable Name of the expansion variable.
 * @param center Expansion center expression.
 * @param context Context containing symbol values.
 * @param environment Mathematical environment containing series settings.
 *
 * @return A @ref math::PreparedAst containing the generated series.
 */
[[nodiscard]]
math::PreparedAst series(const math::PreparedAst& prepared,
                         const std::string& variable,
                         const std::string& center,
                         const core::Context& context,
                         const config::MathEnvironment& environment);

}  // namespace numathap::backend::series
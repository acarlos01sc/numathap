/**
 * @file evaluate.hpp
 * @brief Provides the entry point for expression evaluation.
 *
 * This header defines the primary interface for triggering the evaluation
 * of a prepared expression AST within a specific environment.
 */
#pragma once

#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/math/PreparedAst.hpp"

namespace numathap::backend {

/**
 * @brief Evaluates a prepared mathematical expression within a given context.
 *
 * This function performs the actual computation of the AST provided by the
 * @ref math::PreparedAst. It uses the symbols and definitions stored in the
 * provided @ref core::Context to resolve identifiers and compute the final
 * result.
 *
 * @note The evaluation process is strictly read-only regarding the @p
 * expression and @p context.
 *
 * @param expression The prepared AST representing the mathematical expression.
 * @param context The execution context containing variable definitions and
 *                runtime environment settings.
 *
 * @return The evaluated result as a @ref core::Value.
 *
 * @throw std::logic_error If the AST is malformed or evaluation fails due to
 *                         undefined symbols or mathematical errors.
 *
 * @see numathap::math::PreparedAst
 */
[[nodiscard]]
core::Value evaluate(const math::PreparedAst& expression,
                     const core::Context& context);

}  // namespace numathap::backend
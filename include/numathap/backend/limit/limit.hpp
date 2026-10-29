/**
 * @file limit.hpp
 * @brief Defines the interface for the limit backend.
 */

#pragma once

#include <string>

#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/math/PreparedAst.hpp"

namespace numathap::backend::limit {
/**
 * @brief Represents the outcome of a limit calculation.
 */
enum class LimitStatus {
    /** @brief The limit exists and is finite. */
    Finite,
    /** @brief The limit is positive infinity. */
    PositiveInfinity,
    /** @brief The limit is negative infinity. */
    NegativeInfinity,
    /** @brief The limit does not exist. */
    DoesNotExist,
    /** @brief The calculation could not be completed successfully. */
    Failed
};

/**
 * @brief Represents the result of a limit calculation.
 *
 * The status determines how the value should be interpreted.
 * When the status is not Finite, the value should not be assumed
 * to represent a finite limit.
 */
struct LimitResult {
    /** @brief The computed limit value, if applicable. */
    core::Value value{};
    /** @brief The outcome of the limit calculation. */
    LimitStatus status{LimitStatus::Failed};
};

/**
 * @brief Calculate the limit of an expression.
 *
 * @param expression Prepared mathematical expression.
 * @param variable Name of the variable with respect to which the limit
 *                 is calculated.
 * @param point Target point. For finite points, a trailing '+' or '-'
 *              specifies right-hand or left-hand approach, respectively.
 *              Use '+inf' or '-inf' for limits at positive or negative
 *              infinity.
 * @return LimitResult of the limit.
 */
[[nodiscard]]
LimitResult limit(const math::PreparedAst& expression,
                  const std::string& variable, const std::string& point);

/**
 * @brief Calculate the limit of an expression using a context.
 *
 * @param expression Prepared mathematical expression.
 * @param variable Name of the variable with respect to which the limit
 *                 is calculated.
 * @param point Target point. For finite points, a trailing '+' or '-'
 *              specifies right-hand or left-hand approach, respectively.
 *              Use '+inf' or '-inf' for limits at positive or negative
 *              infinity.
 * @param context Context providing values for other variables in the
 *                expression.
 * @return LimitResult of the limit.
 */
[[nodiscard]]
LimitResult limit(const math::PreparedAst& expression,
                  const std::string& variable, const std::string& point,
                  core::Context& context);

}  // namespace numathap::backend::limit
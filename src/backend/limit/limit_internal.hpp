#pragma once

#include <string>

#include "numathap/backend/limit/limit.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"

namespace numathap::backend::limit::internal {

/** @brief Direction from which the limit point is approached.*/
enum class Approach {
    /** @brief Approach the point from both sides. */
    Bilateral,
    /** @brief Approach the point from values greater than the point. */
    FromRight,
    /** @brief Approach the point from values smaller than the point. */
    FromLeft
};

/**
 * @brief Evaluate a limit by analyzing a supported algebraic singularity.
 *
 * @param normalized Normalized expression to analyze.
 * @param variable Name of the variable approaching the point.
 * @param pointValue Finite real value of the approach point.
 * @param approach Direction from which the point is approached.
 * @param context Context used to evaluate the regular terms.
 * @return The resulting limit status and value. Returns a failed result when
 *         the expression is not covered by the supported algebraic analysis.
 *
 * @details The current implementation supports one singular term formed from
 * an affine base raised to a negative integer exponent, together with regular
 * additive terms that evaluate to finite real values at the approach point.
 */
LimitResult calculateAlgebraicSingularity(const math::PreparedAst& normalized,
                                          const std::string& variable,
                                          const core::Value& pointValue,
                                          Approach approach,
                                          core::Context& context);

}  // namespace numathap::backend::limit::internal
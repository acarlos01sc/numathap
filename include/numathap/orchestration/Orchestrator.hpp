#pragma once

#include "numathap/config/MathEnvironment.hpp"
#include "numathap/math/MathNode.hpp"

namespace numathap::orchestration {

/**
 * @brief Prepares a Math-AST for further processing.
 *
 * The Orchestrator coordinates the preparation of a Math-AST using
 * the capabilities enabled in a MathEnvironment.
 *
 * The input is a Math-AST produced by MathAstBuilder.
 * The output is a Prepared AST, represented currently by the same
 * MathNode hierarchy.
 *
 * Future responsibilities may include:
 *
 * - symbolic simplification;
 * - constant folding;
 * - semantic normalization;
 * - capability-specific preparation;
 * - coordination of mathematical transformations.
 *
 * The Orchestrator does not evaluate expressions and does not
 * depend on parser-level AST nodes.
 */
class Orchestrator {
   public:
    /**
     * @brief Prepares a Math-AST.
     *
     * The current implementation performs no transformation and
     * returns the received tree unchanged. This establishes the
     * architectural boundary where future preparation stages will
     * be introduced.
     *
     * @param ast Math-AST to be prepared.
     * @param environment Resolved execution environment.
     *
     * @return Prepared AST.
     */
    [[nodiscard]]
    math::MathNodePtr prepare(
        math::MathNodePtr ast,
        const config::MathEnvironment& environment) const;
};

}  // namespace numathap::orchestration
#pragma once

#include "numathap/math/MathNode.hpp"

namespace numathap::orchestration {

/**
 * @brief Prepares a Math-AST for further processing.
 *
 * The Orchestrator is the coordination point between the semantic
 * representation of an expression (Math-AST) and the capabilities
 * that will operate on it.
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
     *
     * @return Prepared AST.
     */
    [[nodiscard]]
    math::MathNodePtr prepare(math::MathNodePtr ast) const;
};

}  // namespace numathap::orchestration
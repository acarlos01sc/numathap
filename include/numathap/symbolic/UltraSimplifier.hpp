/**
 * @file UltraSimplifier.hpp
 * @brief Performs aggressive local algebraic simplifications on Math-ASTs.
 *
 * Unlike Simplifier, UltraSimplifier may apply transformations that can
 * change the global domain of an expression, such as cancelling common
 * factors in a quotient.
 *
 * It is intended for contexts such as Taylor-series construction, where
 * local algebraic equivalence is useful for controlling AST growth.
 */

#pragma once

#include "numathap/math/MathAst.hpp"
#include "numathap/math/MathNode.hpp"

namespace numathap::symbolic {

/**
 * @brief Performs aggressive algebraic simplifications on a Math-AST.
 *
 * The original Math-AST is never modified. A new Math-AST is returned.
 *
 * The first implementation focuses on cancellation of structurally
 * equivalent factors between the numerator and denominator.
 *
 * Examples:
 *
 *     x / x             -> 1
 *     (x * y) / x       -> y
 *     (x * y) / (x * z) -> y / z
 *
 * @warning
 * These transformations may change the global domain of the expression.
 * Therefore UltraSimplifier should only be used in contexts where such
 * local algebraic transformations are mathematically acceptable.
 */
class UltraSimplifier {
   public:
    /**
     * @brief Simplifies a Math-AST using aggressive algebraic rules.
     *
     * @param mathAst Source Math-AST.
     * @return A new simplified Math-AST.
     */
    [[nodiscard]]
    math::MathAst simplify(const math::MathAst& mathAst) const;

   private:
    /**
     * @brief Recursively simplifies a node.
     */
    [[nodiscard]]
    math::MathNodePtr simplifyNode(const math::MathNode& node) const;

    [[nodiscard]]
    math::MathNodePtr simplifyUnary(const math::UnaryNode& node) const;

    [[nodiscard]]
    math::MathNodePtr simplifyBinary(const math::BinaryNode& node) const;

    [[nodiscard]]
    math::MathNodePtr simplifyFunction(const math::FunctionNode& node) const;

    /**
     * @brief Simplifies a binary operation after its children were processed.
     */
    [[nodiscard]]
    math::MathNodePtr simplifyBinaryNode(math::BinaryOp op,
                                         math::MathNodePtr left,
                                         math::MathNodePtr right) const;

    /**
     * @brief Cancels common factors in a quotient.
     */
    [[nodiscard]]
    math::MathNodePtr cancelCommonFactors(math::MathNodePtr numerator,
                                          math::MathNodePtr denominator) const;

    /**
     * @brief Extracts multiplicative factors from a node.
     *
     * For example:
     *
     *     A * (B * C)
     *
     * becomes:
     *
     *     [A, B, C]
     */
    void collectFactors(const math::MathNode& node,
                        std::vector<math::MathNodePtr>& factors) const;

    /**
     * @brief Rebuilds a multiplication tree from a list of factors.
     */
    [[nodiscard]]
    math::MathNodePtr buildProduct(
        std::vector<math::MathNodePtr> factors) const;

    /**
     * @brief Tests structural equivalence between two AST nodes.
     */
    [[nodiscard]]
    bool equivalent(const math::MathNode& left,
                    const math::MathNode& right) const;

    /**
     * @brief Returns true when the node represents the numeric zero.
     *
     * Zero is never cancelled as a common factor.
     */
    [[nodiscard]]
    bool isZero(const math::MathNode& node) const;

    /**
     * @brief Tests whether the node represents the numeric one.
     */
    [[nodiscard]]
    bool isOne(const math::MathNode& node) const;
};

}  // namespace numathap::symbolic
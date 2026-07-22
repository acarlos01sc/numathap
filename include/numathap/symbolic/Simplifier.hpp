#pragma once

#include "numathap/math/MathAst.hpp"
#include "numathap/math/MathNode.hpp"

namespace numathap::symbolic {

/**
 * @brief Performs symbolic simplifications on a Math-AST.
 *
 * The Simplifier applies algebraic identities without performing any
 * numerical evaluation. The original Math-AST is never modified; instead,
 * a new simplified Math-AST is returned.
 */
class Simplifier {
public:
    /**
     * @brief Simplifies a Math-AST.
     *
     * @param mathAst The source Math-AST.
     * @return A simplified Math-AST.
     */
    [[nodiscard]]
    math::MathAst simplify(const math::MathAst& mathAst) const;

private:
    //
    // Recursive simplification
    //

    [[nodiscard]]
    math::MathNodePtr simplifyNode(const math::MathNode& node) const;

    [[nodiscard]]
    math::MathNodePtr simplifyNumber(const math::NumberNode& node) const;

    [[nodiscard]]
    math::MathNodePtr simplifySymbol(const math::SymbolNode& node) const;

    [[nodiscard]]
    math::MathNodePtr simplifyUnary(const math::UnaryNode& node) const;

    [[nodiscard]]
    math::MathNodePtr simplifyBinary(const math::BinaryNode& node) const;

    [[nodiscard]]
    math::MathNodePtr simplifyFunction(const math::FunctionNode& node) const;

    //
    // Simplification rules
    //

    [[nodiscard]]
    math::MathNodePtr simplifyUnaryNode(
        math::UnaryOp op,
        math::MathNodePtr operand) const;

    [[nodiscard]]
    math::MathNodePtr simplifyBinaryNode(
        math::BinaryOp op,
        math::MathNodePtr left,
        math::MathNodePtr right) const;

    //
    // Pattern recognition
    //

    [[nodiscard]]
    bool isZero(const math::MathNode& node) const;

    [[nodiscard]]
    bool isOne(const math::MathNode& node) const;

    [[nodiscard]]
    bool isUnaryMinusOne(const math::MathNode& node) const;
};

} // namespace numathap::symbolic
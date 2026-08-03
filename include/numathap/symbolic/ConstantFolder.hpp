/**
 * @file ConstantFolder.hpp
 * @brief Defines the ConstantFolder class responsible for transforming Math-ASTs.
 *
 * This file contains the declaration of the ConstantFolder, which acts as a
 * bridge between raw mathematical expressions and prepared structures optimized
 * for evaluation.
 */
#pragma once

#include "numathap/math/MathAst.hpp"
#include "numathap/math/MathNode.hpp"

namespace numathap::symbolic {

/**
 * @brief Performs constant folding on a Math-AST.
 *
 * The ConstantFolder evaluates subexpressions composed entirely of numeric
 * literals and replaces them with their computed value. The original Math-AST
 * is never modified; instead, a new Math-AST is returned.
 *
 * Examples:
 *   2 + 3       -> 5
 *   (4 - 1) * x -> 3 * x
 *
 * Algebraic identities (such as x + 0 -> x) are intentionally outside the
 * scope of this class and are handled by Simplifier.
 */
class ConstantFolder {
   public:
    /**
     * @brief Evaluates constant subexpressions in a Math-AST.
     *
     * @param mathAst The source Math-AST.
     * @return A Math-AST with constant subexpressions folded.
     */
    [[nodiscard]]
    math::MathAst fold(const math::MathAst& mathAst) const;

   private:
    //
    // Recursive folding
    //

    [[nodiscard]]
    math::MathNodePtr foldNode(const math::MathNode& node) const;

    [[nodiscard]]
    math::MathNodePtr foldNumber(const math::NumberNode& node) const;

    [[nodiscard]]
    math::MathNodePtr foldSymbol(const math::SymbolNode& node) const;

    [[nodiscard]]
    math::MathNodePtr foldUnary(const math::UnaryNode& node) const;

    [[nodiscard]]
    math::MathNodePtr foldBinary(const math::BinaryNode& node) const;

    [[nodiscard]]
    math::MathNodePtr foldFunction(const math::FunctionNode& node) const;

    //
    // Folding rules
    //

    [[nodiscard]]
    math::MathNodePtr foldUnaryNode(math::UnaryOp op,
                                    math::MathNodePtr operand) const;

    [[nodiscard]]
    math::MathNodePtr foldBinaryNode(math::BinaryOp op,
                                     math::MathNodePtr left,
                                     math::MathNodePtr right) const;

    //
    // Pattern recognition
    //

    [[nodiscard]]
    bool isNumber(const math::MathNode& node) const;
};

}  // namespace numathap::symbolic
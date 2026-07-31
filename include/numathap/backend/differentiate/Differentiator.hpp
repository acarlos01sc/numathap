/**
 * @file Differentiator.hpp
 * @brief Defines the Differentiator backend.
 */
#pragma once

#include <string_view>

#include "numathap/math/MathAst.hpp"
#include "numathap/math/MathNode.hpp"
#include "numathap/math/PreparedAst.hpp"

namespace numathap::backend::differentiate {

/**
 * @brief Performs symbolic differentiation of a prepared mathematical AST.
 *
 * The Differentiator traverses the MathNode tree of a PreparedAst and builds
 * a new MathNode tree representing its derivative with respect to a variable.
 *
 * The resulting tree is simplified as an intrinsic part of the differentiation
 * pipeline.
 *
 * Function-specific differentiation rules are delegated to a mathematical
 * library-specific differentiator, such as CMathDifferentiator.
 */
class Differentiator {
   public:
    Differentiator() = default;
    ~Differentiator() = default;

    /**
     * @brief Differentiates a prepared AST with respect to a variable.
     *
     * @param ast The prepared AST to differentiate.
     * @param variable The differentiation variable.
     * @return A PreparedAst representing the simplified derivative.
     *
     * @throws std::logic_error If the input AST is empty.
     * @throws std::invalid_argument If the differentiation variable is empty.
     */
    [[nodiscard]]
    math::PreparedAst differentiate(const math::PreparedAst& ast,
                                    std::string_view variable) const;

   private:
    /**
     * @brief Recursively differentiates a MathNode.
     *
     * @param node The node to differentiate.
     * @param variable The differentiation variable.
     * @return A newly constructed node representing the derivative.
     */
    [[nodiscard]]
    math::MathNodePtr differentiateNode(const math::MathNode& node,
                                        std::string_view variable) const;

    /**
     * @brief Differentiates a numeric literal.
     *
     * @return The constant zero.
     */
    [[nodiscard]]
    math::MathNodePtr differentiateNumber(const math::NumberNode& node) const;

    /**
     * @brief Differentiates a symbol.
     *
     * Returns one if the symbol is the differentiation variable and zero
     * otherwise.
     */
    [[nodiscard]]
    math::MathNodePtr differentiateSymbol(const math::SymbolNode& node,
                                          std::string_view variable) const;

    /**
     * @brief Differentiates a unary expression.
     */
    [[nodiscard]]
    math::MathNodePtr differentiateUnary(const math::UnaryNode& node,
                                         std::string_view variable) const;

    /**
     * @brief Differentiates a binary expression.
     */
    [[nodiscard]]
    math::MathNodePtr differentiateBinary(const math::BinaryNode& node,
                                          std::string_view variable) const;

    /**
     * @brief Differentiates a mathematical function.
     *
     * Function-specific rules are delegated to the configured mathematical
     * differentiator.
     */
    [[nodiscard]]
    math::MathNodePtr differentiateFunction(const math::FunctionNode& node,
                                            std::string_view variable) const;

    /**
     * @brief Differentiates a power expression.
     *
     * The derivatives of the base and exponent are computed recursively and
     * passed to the configured mathematical differentiator, which constructs
     * the general derivative rule for a power expression.
     */
    math::MathNodePtr differentiatePower(const math::BinaryNode& node,
                                         std::string_view variable) const;
};

}  // namespace numathap::backend::differentiate
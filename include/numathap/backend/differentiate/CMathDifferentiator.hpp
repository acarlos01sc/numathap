/**
 * @file CMathDifferentiator.hpp
 * @brief Defines the CMathDifferentiator backend.
 */
#pragma once

#include "numathap/math/MathNode.hpp"

namespace numathap::backend::differentiate {

/**
 * @brief Differentiation rules specific to the C++ standard math library.
 *
 * CMathDifferentiator provides differentiation rules whose representation
 * depends on the mathematical library in use. It is used by the generic
 * Differentiator for mathematical functions and power expressions.
 *
 * The generic differentiation rules for arithmetic operators remain in
 * Differentiator.
 */
class CMathDifferentiator final {
   public:
    CMathDifferentiator() = default;
    ~CMathDifferentiator() = default;

    /**
     * @brief Differentiates a mathematical function.
     *
     * The derivative of the function argument is supplied by the generic
     * Differentiator, allowing this component to apply the corresponding
     * function-specific rule and the chain rule.
     *
     * @param node Function node to differentiate.
     * @param argumentDerivative Derivative of the function argument.
     * @return Root node of the derivative expression.
     *
     * @throw std::logic_error If the function is not supported.
     */
    [[nodiscard]]
    math::MathNodePtr differentiate(
        const math::FunctionNode& node,
        math::MathNodePtr argumentDerivative) const;

    /**
     * @brief Differentiates a power expression.
     *
     * The derivatives of the base and exponent are supplied by the generic
     * Differentiator. The specific rule may require functions provided by
     * the selected mathematical library, such as the natural logarithm.
     *
     * @param node Power node to differentiate.
     * @param baseDerivative Derivative of the power base.
     * @param exponentDerivative Derivative of the power exponent.
     * @return Root node of the derivative expression.
     *
     * @throw std::logic_error If power differentiation is not implemented.
     */
    [[nodiscard]]
    math::MathNodePtr differentiate(
        const math::BinaryNode& node,
        math::MathNodePtr baseDerivative,
        math::MathNodePtr exponentDerivative) const;
};

}  // namespace numathap::backend::differentiate
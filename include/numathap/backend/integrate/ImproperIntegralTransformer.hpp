/**
 * @file ImproperIntegralTransformer.hpp
 * @brief Defines the transformation of improper integrals using tangent
 * substitution.
 */

#pragma once

#include <string>

#include "numathap/config/MathEnvironment.hpp"
#include "numathap/math/PreparedAst.hpp"

namespace numathap::backend::integrate {

/**
 * @brief Result of an improper-integral variable transformation.
 *
 * The transformed PreparedAst is independent from the original AST and uses
 * an internally generated integration variable.
 */
struct TransformedIntegral {
    math::PreparedAst prepared;  ///< The transformed expression AST.
    std::string variable;  ///< The internal variable name substituted into the
                           ///< expression.
};

/**
 * @brief Transforms an improper integral using the tangent substitution.
 *
 * The transformation is:
 *
 *     x = tan(u)
 *
 * with:
 *
 *     dx = 1 / cos(u)^2 du
 *
 * Therefore an expression f(x) is transformed into:
 *
 *     f(tan(u)) / cos(u)^2
 *
 * The original PreparedAst is never modified.
 */
class ImproperIntegralTransformer {
   public:
    /**
     * @brief Transforms the expression of an improper integral.
     *
     * @param prepared Original prepared AST. It remains immutable.
     * @param variable Original integration variable.
     * @param environment Mathematical environment used to prepare the
     * transformed AST.
     *
     * @return A transformed PreparedAst and its generated internal variable.
     */
    [[nodiscard]]
    TransformedIntegral transform(
        const math::PreparedAst& prepared, const std::string& variable,
        const config::MathEnvironment& environment) const;

   private:
    /**
     * @brief Generates an internal variable name that does not collide with
     * symbols already present in the expression.
     */
    [[nodiscard]]
    std::string generateInternalVariable(
        const math::PreparedAst& prepared) const;

    /**
     * @brief Checks whether a symbol occurs anywhere in the AST.
     */
    [[nodiscard]]
    bool containsSymbol(const math::MathNode& node,
                        const std::string& symbol) const;

    /**
     * @brief Recursively clones and transforms a MathNode.
     */
    [[nodiscard]]
    math::MathNodePtr transformNode(const math::MathNode& node,
                                    const std::string& integrationVariable,
                                    const std::string& internalVariable) const;

    /**
     * @brief Creates tan(internalVariable).
     */
    [[nodiscard]]
    math::MathNodePtr makeTangentNode(
        const std::string& internalVariable) const;

    /**
     * @brief Creates the Jacobian 1 / cos(internalVariable)^2.
     */
    [[nodiscard]]
    math::MathNodePtr makeJacobianNode(
        const std::string& internalVariable) const;

    /**
     * @brief Wraps the transformed expression with the Jacobian.
     */
    [[nodiscard]]
    math::MathNodePtr applyJacobian(math::MathNodePtr transformedExpression,
                                    const std::string& internalVariable) const;
};

}  // namespace numathap::backend::integrate
#include "limit_internal.hpp"

#include <exception>
#include <limits>
#include <string>
#include <vector>

#include "numathap/backend/BackendSupport.hpp"
#include "numathap/backend/evaluate.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/math/MathNode.hpp"
#include "numathap/numeric/Real.hpp"


namespace numathap::backend::limit::internal {


namespace {

struct AffineExpression {
    numeric::Real coefficient;
    numeric::Real constant;
};

struct SingularTerm {
    numeric::Real coefficient;
    AffineExpression base;
    unsigned int exponent;
};

struct ExpressionTerm {
    const math::MathNode* node;
    int sign;
};

struct SingularExpression {
    SingularTerm singular;
    std::vector<ExpressionTerm> regularTerms;
};


bool extractAffineExpression(const math::MathNode& node,
                             const std::string& variable,
                             AffineExpression& result) {
    using numeric::Real;

    // A number represents a constant affine expression (0*x + b).
    if (const auto* number = dynamic_cast<const math::NumberNode*>(&node)) {
        try {
            const Real value(number->value.c_str());

            if (!value.isFinite()) {
                return false;
            }

            result = AffineExpression{Real(0.0), value};
            return true;

        } catch (const std::exception&) {
            return false;
        }
    }

    // Accept only the variable whose limit is being computed.
    if (const auto* symbol = dynamic_cast<const math::SymbolNode*>(&node)) {
        if (symbol->name != variable) {
            return false;
        }

        result = AffineExpression{Real(1.0), Real(0.0)};
        return true;
    }

    // Handle unary operations.
    if (const auto* unary = dynamic_cast<const math::UnaryNode*>(&node)) {
        AffineExpression operand;

        if (!extractAffineExpression(*unary->operand, variable, operand)) {
            return false;
        }

        if (unary->op == math::UnaryOp::Minus) {
            operand.coefficient = -operand.coefficient;
            operand.constant = -operand.constant;
        }

        result = operand;
        return true;
    }

    // Handle binary operations.
    if (const auto* binary = dynamic_cast<const math::BinaryNode*>(&node)) {
        AffineExpression lhs;
        AffineExpression rhs;

        if (!extractAffineExpression(*binary->left, variable, lhs) ||
            !extractAffineExpression(*binary->right, variable, rhs)) {
            return false;
        }

        switch (binary->op) {
            case math::BinaryOp::Add:
                result = AffineExpression{lhs.coefficient + rhs.coefficient,
                                          lhs.constant + rhs.constant};
                return true;

            case math::BinaryOp::Subtract:
                result = AffineExpression{lhs.coefficient - rhs.coefficient,
                                          lhs.constant - rhs.constant};
                return true;

            case math::BinaryOp::Multiply:
                // Pelo menos um dos operandos precisa ser constante.
                if (lhs.coefficient != Real(0.0) &&
                    rhs.coefficient != Real(0.0)) {
                    return false;
                }

                if (lhs.coefficient == Real(0.0)) {
                    result = AffineExpression{rhs.coefficient * lhs.constant,
                                              rhs.constant * lhs.constant};
                } else {
                    result = AffineExpression{lhs.coefficient * rhs.constant,
                                              lhs.constant * rhs.constant};
                }

                return true;

            case math::BinaryOp::Divide:
                // O denominador precisa ser uma constante não nula.
                if (rhs.coefficient != Real(0.0) || rhs.constant == Real(0.0)) {
                    return false;
                }

                result = AffineExpression{lhs.coefficient / rhs.constant,
                                          lhs.constant / rhs.constant};
                return true;

            case math::BinaryOp::Power:
                return false;
        }
    }

    // FunctionNode and all other node types are not affine expressions.
    return false;
}

bool isKnownRegularTerm(const math::MathNode& node,
                        const std::string& variable) {
    AffineExpression expression;

    if (!extractAffineExpression(node, variable, expression)) {
        return false;
    }

    return expression.coefficient.isFinite() &&
           expression.constant.isFinite();
}

bool extractNegativeIntegerExponent(const math::MathNode& node,
                                    const std::string& variable,
                                    unsigned int& exponent) {
    AffineExpression expression;

    if (!extractAffineExpression(node, variable, expression)) {
        return false;
    }

    // The exponent must be constant.
    if (expression.coefficient != numeric::Real(0.0)) {
        return false;
    }

    const numeric::Real& value = expression.constant;

    if (!value.isFinite() || value.sign() >= 0 || !value.isInteger()) {
        return false;
    }

    // Convert the absolute value of the exponent to unsigned int.
    const auto magnitude = -value.value();

    const auto maxExponent = static_cast<numeric::Real::Storage>(
        std::numeric_limits<unsigned int>::max());

    if (magnitude < 1 || magnitude >= maxExponent + 1.0) {
        return false;
    }

    exponent = static_cast<unsigned int>(magnitude);

    return true;
}

bool collectSingularFactors(const math::MathNode& node,
                            const std::string& variable, SingularTerm& result,
                            bool& foundSingular) {
    using numeric::Real;

    // Initialize the local result.
    result = SingularTerm{Real(1.0), AffineExpression{Real(0.0), Real(0.0)}, 0};

    foundSingular = false;

    // Handle unary operations.
    if (const auto* unary = dynamic_cast<const math::UnaryNode*>(&node)) {
        SingularTerm operand;
        bool operandHasSingular = false;

        if (!collectSingularFactors(*unary->operand, variable, operand,
                                    operandHasSingular)) {
            return false;
        }

        if (unary->op == math::UnaryOp::Minus) {
            operand.coefficient = -operand.coefficient;
        }

        result = operand;
        foundSingular = operandHasSingular;

        return result.coefficient.isFinite();
    }

    // Recursively process products.
    if (const auto* binary = dynamic_cast<const math::BinaryNode*>(&node)) {
        if (binary->op == math::BinaryOp::Multiply) {
            SingularTerm lhs;
            SingularTerm rhs;

            bool lhsHasSingular = false;
            bool rhsHasSingular = false;

            if (!collectSingularFactors(*binary->left, variable, lhs,
                                        lhsHasSingular) ||
                !collectSingularFactors(*binary->right, variable, rhs,
                                        rhsHasSingular)) {
                return false;
            }

            // This stage supports at most one singular power.
            if (lhsHasSingular && rhsHasSingular) {
                return false;
            }

            result.coefficient = lhs.coefficient * rhs.coefficient;

            if (lhsHasSingular) {
                result.base = lhs.base;
                result.exponent = lhs.exponent;
            } else if (rhsHasSingular) {
                result.base = rhs.base;
                result.exponent = rhs.exponent;
            }

            foundSingular = lhsHasSingular || rhsHasSingular;

            return result.coefficient.isFinite();
        }

        // Identify a power with a negative integer exponent.
        if (binary->op == math::BinaryOp::Power) {
            AffineExpression base;
            unsigned int exponent{};

            if (!extractAffineExpression(*binary->left, variable, base)) {
                return false;
            }

            if (base.coefficient == Real(0.0)) {
                return false;
            }

            if (!extractNegativeIntegerExponent(*binary->right, variable,
                                                exponent)) {
                return false;
            }

            result.base = base;
            result.exponent = exponent;
            foundSingular = true;

            return true;
        }
    }

    // Any other factor must be constant.
    AffineExpression factor;

    if (!extractAffineExpression(node, variable, factor)) {
        return false;
    }

    if (factor.coefficient != Real(0.0)) {
        return false;
    }

    result.coefficient = factor.constant;

    return result.coefficient.isFinite();
}

bool extractSingularTerm(const math::MathNode& node,
                         const std::string& variable, SingularTerm& result) {
    result = SingularTerm{
        numeric::Real(1.0),
        AffineExpression{numeric::Real(0.0), numeric::Real(0.0)}, 0};

    bool foundSingular = false;

    if (!collectSingularFactors(node, variable, result, foundSingular)) {
        return false;
    }

    return foundSingular && result.coefficient.isFinite() &&
           result.coefficient.sign() != 0;
}

bool collectExpressionTerms(const math::MathNode& node, int sign,
                            std::vector<ExpressionTerm>& terms) {
    // Handle unary signs.
    if (const auto* unary = dynamic_cast<const math::UnaryNode*>(&node)) {
        const int operandSign =
            unary->op == math::UnaryOp::Minus ? -sign : sign;

        return collectExpressionTerms(*unary->operand, operandSign, terms);
    }

    // Handle addition and subtraction.
    if (const auto* binary = dynamic_cast<const math::BinaryNode*>(&node)) {
        if (binary->op == math::BinaryOp::Add) {
            return collectExpressionTerms(*binary->left, sign, terms) &&
                   collectExpressionTerms(*binary->right, sign, terms);
        }

        if (binary->op == math::BinaryOp::Subtract) {
            return collectExpressionTerms(*binary->left, sign, terms) &&
                   collectExpressionTerms(*binary->right, -sign, terms);
        }
    }

    terms.push_back(ExpressionTerm{&node, sign});
    return true;
}

bool extractSingularExpression(const math::MathNode& node,
                               const std::string& variable,
                               SingularExpression& result) {
    std::vector<ExpressionTerm> terms;

    if (!collectExpressionTerms(node, 1, terms)) {
        return false;
    }

    bool foundSingular = false;

    for (const auto& term : terms) {
        SingularTerm singular;

        if (extractSingularTerm(*term.node, variable, singular)) {
            // This stage supports at most one singular term.
            if (foundSingular) {
                return false;
            }

            if (term.sign < 0) {
                singular.coefficient = -singular.coefficient;
            }

            result.singular = singular;
            foundSingular = true;

        } else if (isKnownRegularTerm(*term.node, variable)) {
            result.regularTerms.push_back(term);
        } else {
            // The term can not be classified safely.
            return false;
        }
    }

    return foundSingular && result.singular.coefficient.isFinite() &&
           result.singular.coefficient.sign() != 0;
}


}

/**
 * @brief Determine a limit by analyzing a supported algebraic singularity.
 *
 * @param normalized Normalized expression to analyze.
 * @param variable Name of the variable approaching the point.
 * @param pointValue Finite real approach point.
 * @param approach Direction from which the point is approached.
 * @param context Evaluation context used for the regular terms.
 * @return A positive- or negative-infinity result when the supported
 *         singularity diverges; a does-not-exist result for an odd-power
 *         bilateral singularity; or a failed result when the expression is
 *         outside the supported form.
 *
 * @details The current analysis recognizes a single singular term consisting
 * of a non-zero constant coefficient multiplied by an affine base raised to
 * a negative integer exponent. The remaining additive terms must be
 * evaluable as finite real values at the approach point.
 */
LimitResult calculateAlgebraicSingularity(const math::PreparedAst& normalized,
                                          const std::string& variable,
                                          const core::Value& pointValue,
                                          Approach approach,
                                          core::Context& context) {
    using numeric::Real;

    SingularExpression expression;

    // Separate the singular term from the regular terms.
    if (!extractSingularExpression(*normalized.root(), variable, expression)) {
        return LimitResult{};
    }

    const SingularTerm& singular = expression.singular;

    // The approach point must make the singular power's base equal to zero.
    const Real baseAtPoint =
        singular.base.coefficient * pointValue.real() + singular.base.constant;

    if (baseAtPoint != Real(0.0)) {
        return LimitResult{};
    }

    // Evaluate the regular terms at the approach point.
    context.setValue(variable, pointValue.str());

    core::Value regularValue(Real(0.0));

    for (const auto& term : expression.regularTerms) {
        math::PreparedAst regularAst(normalized.expression(),
                                     BackendSupport::cloneNode(*term.node),
                                     normalized.environment());

        core::Value termValue;

        try {
            termValue = backend::evaluate(regularAst, context);
        } catch (const std::exception&) {
            return LimitResult{};
        }

        // In this implementation, the regular part must
        // evaluate to finite real values.
        if (!termValue.isReal() || !termValue.real().isFinite()) {
            return LimitResult{};
        }

        if (term.sign < 0) {
            termValue = -termValue;
        }

        regularValue = regularValue + termValue;

        if (!regularValue.isReal() || !regularValue.real().isFinite()) {
            return LimitResult{};
        }
    }

    // Determine the sign of the divergence.
    int divergenceSign = singular.coefficient.sign();

    // For an odd exponent, the sign depends on the side
    // from which the base approaches zero.
    if (singular.exponent % 2 != 0) {
        if (approach == Approach::Bilateral) {
            return LimitResult{core::Value{}, LimitStatus::DoesNotExist};
        }

        int directionSign = approach == Approach::FromRight ? 1 : -1;

        const int baseDirectionSign =
            singular.base.coefficient.sign() * directionSign;

        divergenceSign *= baseDirectionSign;
    }

    if (divergenceSign > 0) {
        return LimitResult{core::Value{}, LimitStatus::PositiveInfinity};
    }

    return LimitResult{core::Value{}, LimitStatus::NegativeInfinity};
}

}  // namespace numathap::backend::limit::internal
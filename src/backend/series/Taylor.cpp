/**
 * @file Taylor.cpp
 * @brief Implements the Taylor series expansion.
 */

#include "numathap/backend/series/Taylor.hpp"

#include <string>
#include <vector>

#include "numathap/backend/series/SeriesDag.hpp"
#include "numathap/backend/BackendSupport.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/math/MathNode.hpp"
#include "numathap/math/PreparedAst.hpp"
#include "numathap/symbolic/Simplifier.hpp"

namespace numathap::backend::series {

namespace {

using math::BinaryNode;
using math::BinaryOp;
using math::FunctionNode;
using math::MathNodePtr;
using math::NumberNode;
using math::SymbolNode;

/**
 * @brief Builds the shared "(variable - center)" node used by every term
 *        of degree >= 1.
 */
MathNodePtr buildOffsetNode(
    const std::string& variable,
    const core::Value& center) {
    return std::make_unique<BinaryNode>(
        BinaryOp::Subtract,
        std::make_unique<SymbolNode>(variable),
        std::make_unique<NumberNode>(center.str()));
}

/**
 * @brief Builds the factorial node for a given degree.
 *
 * The factorial is deliberately kept as a FunctionNode so that the
 * generated Taylor expression explicitly represents
 *
 *     f^(n)(a) / n!
 */
MathNodePtr buildFactorialNode(std::size_t degree) {
    std::vector<MathNodePtr> arguments;

    arguments.push_back(
        std::make_unique<NumberNode>(
            std::to_string(degree)));

    return std::make_unique<FunctionNode>(
        "factorial",
        std::move(arguments));
}

/**
 * @brief Builds the k-th Taylor term:
 *
 *     (f^(k)(center) / k!) * (x - center)^k
 *
 * For k == 0 the term is simply f(center).
 */
MathNodePtr buildTerm(
    const core::Value& derivativeAtCenter,
    const std::string& variable,
    const core::Value& center,
    std::size_t degree) {
    auto derivativeNode =
        std::make_unique<NumberNode>(
            derivativeAtCenter.str());

    if (degree == 0) {
        return derivativeNode;
    }

    auto factorialNode =
        buildFactorialNode(degree);

    auto coefficientNode =
        std::make_unique<BinaryNode>(
            BinaryOp::Divide,
            std::move(derivativeNode),
            std::move(factorialNode));

    auto offset =
        buildOffsetNode(variable, center);

    MathNodePtr powerNode;

    if (degree == 1) {
        powerNode = std::move(offset);
    } else {
        powerNode =
            std::make_unique<BinaryNode>(
                BinaryOp::Power,
                std::move(offset),
                std::make_unique<NumberNode>(
                    std::to_string(degree)));
    }

    return std::make_unique<BinaryNode>(
        BinaryOp::Multiply,
        std::move(coefficientNode),
        std::move(powerNode));
}

}  // namespace

math::PreparedAst Taylor::series(
    const math::PreparedAst& prepared,
    const std::string& variable,
    const core::Value& center,
    const TaylorConfig& config) {
    if (variable.empty()) {
        throw std::invalid_argument(
            "Taylor: expansion variable cannot be empty.");
    }

    /*
     * Build the DAG once.
     *
     * All derivatives will subsequently be evaluated directly on
     * this DAG. No symbolic derivative DAGs are constructed.
     */
    SeriesDag dag(prepared);

    /*
     * Calculate all derivatives simultaneously:
     *
     *     [f(a), f'(a), ..., f^(N)(a)]
     *
     * The values are derivative values, not Taylor coefficients.
     */
    const SeriesDag::DerivativeValues derivatives =
        dag.fastEvaluateDerivativesAt(
            variable,
            center,
            config.order);

    /*
     * Degree-zero term:
     *
     *     f(a)
     */
    MathNodePtr expansion =
        buildTerm(
            derivatives[0],
            variable,
            center,
            0);

    /*
     * Add the remaining Taylor terms:
     *
     *     f^(k)(a) / k! * (x-a)^k
     */
    for (std::size_t degree = 1;
         degree <= config.order;
         ++degree) {
        expansion =
            std::make_unique<BinaryNode>(
                BinaryOp::Add,
                std::move(expansion),
                buildTerm(
                    derivatives[degree],
                    variable,
                    center,
                    degree));
    }

    /*
     * Simplify the generated Taylor expression.
     *
     * This is performed only on the final Taylor expression.
     * No symbolic differentiation is involved.
     */
    math::MathAst expansionAst(
        prepared.expression(),
        std::move(expansion));

    symbolic::Simplifier simplifier;

    auto simplifiedAst =
        simplifier.simplify(expansionAst);

    auto preparedRoot =
        math::MathNodePtr(
            simplifiedAst.root()
                ? BackendSupport::cloneNode(*simplifiedAst.root())
                : nullptr);

    std::string expression =
        "taylor(" +
        prepared.expression() +
        ", " +
        variable +
        ", " +
        center.str() +
        ", " +
        std::to_string(config.order) +
        ")";

    return math::PreparedAst(
        std::move(expression),
        std::move(preparedRoot),
        prepared.environment());
}

}  // namespace numathap::backend::series
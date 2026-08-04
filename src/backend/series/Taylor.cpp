/**
 * @file Taylor.cpp
 * @brief Implements the Taylor series expansion.
 */

#include "numathap/backend/series/Taylor.hpp"

#include <memory>
#include <string>

#include "numathap/backend/differentiate/differentiate.hpp"
#include "numathap/backend/evaluate.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/dispatch/Dispatcher.hpp"
#include "numathap/math/MathNode.hpp"
#include "numathap/math/PreparedAst.hpp"
#include "numathap/symbolic/Simplifier.hpp"

namespace numathap::backend::series {

namespace {

using math::BinaryNode;
using math::BinaryOp;
using math::MathNodePtr;
using math::NumberNode;
using math::SymbolNode;
using math::UnaryNode;

using namespace math;

/**
 * @brief Builds the shared "(variable - center)" node used by every term
 * of degree >= 1.
 */
MathNodePtr buildOffsetNode(const std::string& variable,
                            const core::Value& center) {
    return std::make_unique<BinaryNode>(
        BinaryOp::Subtract, std::make_unique<SymbolNode>(variable),
        std::make_unique<NumberNode>(center.str()));
}

/**
 * @brief Builds the k-th term of the expansion: coefficient * (x - a)^k.
 *
 * For degree == 0 the term is just the coefficient (the "(x - a)^0"
 * factor is elided). For degree == 1 the redundant exponent is also
 * elided, producing "coefficient * (x - a)" instead of
 * "coefficient * (x - a)^1".
 *
 * @param coefficient The already-computed numeric coefficient (i.e.
 * f^(degree)(center) / degree!), as a numeric Value.
 * @param variable Name of the expansion variable.
 * @param center Expansion center.
 * @param degree Degree of this term (0-based).
 *
 * @return The MathNode tree for this term.
 */
MathNodePtr buildTerm(const core::Value& coefficient,
                      const std::string& variable, const core::Value& center,
                      std::size_t degree) {
    MathNodePtr coefficientNode =
        std::make_unique<NumberNode>(coefficient.str());

    if (degree == 0) {
        return coefficientNode;
    }

    MathNodePtr offset = buildOffsetNode(variable, center);

    MathNodePtr powerNode;
    if (degree == 1) {
        powerNode = std::move(offset);
    } else {
        powerNode = std::make_unique<BinaryNode>(
            BinaryOp::Power, std::move(offset),
            std::make_unique<NumberNode>(std::to_string(degree)));
    }

    return std::make_unique<BinaryNode>(
        BinaryOp::Multiply, std::move(coefficientNode), std::move(powerNode));
}

MathNodePtr cloneNode(const MathNode& node) {
    return dispatch::Dispatcher::dispatch(
        node, [](const auto& concreteNode) -> MathNodePtr {
            using NodeType = std::decay_t<decltype(concreteNode)>;

            if constexpr (std::is_same_v<NodeType, NumberNode>) {
                return std::make_unique<NumberNode>(concreteNode.value);

            } else if constexpr (std::is_same_v<NodeType, SymbolNode>) {
                return std::make_unique<SymbolNode>(concreteNode.name);

            } else if constexpr (std::is_same_v<NodeType, UnaryNode>) {
                return std::make_unique<UnaryNode>(
                    concreteNode.op, cloneNode(*concreteNode.operand));

            } else if constexpr (std::is_same_v<NodeType, BinaryNode>) {
                return std::make_unique<BinaryNode>(
                    concreteNode.op, cloneNode(*concreteNode.left),
                    cloneNode(*concreteNode.right));

            } else if constexpr (std::is_same_v<NodeType, FunctionNode>) {
                std::vector<MathNodePtr> arguments;
                arguments.reserve(concreteNode.arguments.size());

                for (const auto& argument : concreteNode.arguments) {
                    arguments.push_back(cloneNode(*argument));
                }

                return std::make_unique<FunctionNode>(concreteNode.name,
                                                      std::move(arguments));

            } else {
                static_assert(std::is_same_v<NodeType, void>,
                              "Differentiator: unsupported MathNode type.");
            }
        });
}

}  // namespace

math::PreparedAst Taylor::series(const math::PreparedAst& prepared,
                                 const std::string& variable,
                                 const core::Value& center,
                                 const TaylorConfig& config) {
    // A single-symbol context: derivatives of `prepared` are evaluated at
    // `variable = center` to obtain each numeric coefficient.
    core::Context context;
    context.setValue(variable, center.str());

    // f(center) — the degree-0 term, i.e. C0.
    core::Value valueAtCenter = numathap::backend::evaluate(prepared, context);
    MathNodePtr expansion = buildTerm(valueAtCenter, variable, center, 0);

    if (config.order > 0) {
        core::Value factorial = core::Value::parse("1");  // 0! = 1

        // First derivative — subsequent ones are obtained by repeatedly
        // differentiating the previous one.
        math::PreparedAst derivative =
            numathap::backend::differentiate::differentiate(prepared, variable);

        for (std::size_t degree = 1; degree <= config.order; ++degree) {
            if (degree > 1) {
                derivative = numathap::backend::differentiate::differentiate(
                    derivative, variable);
            }

            factorial = factorial * core::Value::parse(std::to_string(degree));

            core::Value derivativeAtCenter =
                numathap::backend::evaluate(derivative, context);
            core::Value coefficient = derivativeAtCenter / factorial;

            expansion = std::make_unique<BinaryNode>(
                BinaryOp::Add, std::move(expansion),
                buildTerm(coefficient, variable, center, degree));
        }
    }

    //  std::string expression = "taylor(" + prepared.expression() + ", " +
    //                            variable + ", " + center.str() + ")";

    //  return math::PreparedAst(std::move(expression), std::move(expansion),
    //                            prepared.environment());

    //
    // Simplification is the final optimization pass.
    //
    math::MathAst expansionAst(prepared.expression(), std::move(expansion));

    symbolic::Simplifier simplifier;
    auto simplifiedAst = simplifier.simplify(expansionAst);

    //
    // Clone the simplified tree because MathAst owns its root.
    //
    auto preparedRoot = cloneNode(*simplifiedAst.root());

    std::string expression = "taylor(" + prepared.expression() + ", " +
                             variable + ", " + center.str() + ", " +
                             std::to_string(config.order) + ")";

    return math::PreparedAst(std::move(expression), std::move(preparedRoot),
                             prepared.environment());
}

}  // namespace numathap::backend::series
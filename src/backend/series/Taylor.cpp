/**

* @file Taylor.cpp
* @brief Implements the Taylor series expansion.
  */

#include "numathap/backend/series/Taylor.hpp"

#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "numathap/backend/differentiate/differentiate.hpp"
#include "numathap/backend/evaluate.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/dispatch/Dispatcher.hpp"
#include "numathap/math/MathNode.hpp"
#include "numathap/math/PreparedAst.hpp"
#include "numathap/symbolic/Simplifier.hpp"
#include "numathap/symbolic/UltraSimplifier.hpp"

namespace numathap::backend::series {

namespace {

using math::BinaryNode;
using math::BinaryOp;
using math::FunctionNode;
using math::MathNode;
using math::MathNodePtr;
using math::NumberNode;
using math::SymbolNode;
using math::UnaryNode;

using namespace math;

/**

* @brief Builds the shared "(variable - center)" node used by every term
*
     of degree >= 1.


*/
MathNodePtr buildOffsetNode(const std::string& variable,
                            const core::Value& center) {
    return std::make_unique<BinaryNode>(
        BinaryOp::Subtract, std::make_unique<SymbolNode>(variable),
        std::make_unique<NumberNode>(center.str()));
}

/**

* @brief Builds the factorial node for a given degree.
*
* The factorial is deliberately kept as a FunctionNode instead of being
* evaluated numerically. This preserves the mathematical structure
* f^(n)(a) / n! in the resulting AST.
*
* @param degree Degree of the Taylor term.
*
* @return MathNode representing factorial(degree).
  */
MathNodePtr buildFactorialNode(std::size_t degree) {
    std::vector<MathNodePtr> arguments;
    arguments.push_back(std::make_unique<NumberNode>(std::to_string(degree)));

    return std::make_unique<FunctionNode>("factorial", std::move(arguments));
}

/**

* @brief Builds the k-th term of the Taylor expansion:
*
*
  (f^(k)(center) / k!) * (x - center)^k

*
* For degree == 0 the term is simply f(center).
* For degree == 1 the redundant exponent is omitted.
*
* The derivative value is already evaluated numerically at the expansion
* center, but the factorial remains explicitly represented as a
* factorial(...) FunctionNode in the AST.
*
* @param derivativeAtCenter Numeric value of f^(degree)(center).
* @param variable Name of the expansion variable.
* @param center Expansion center.
* @param degree Degree of this term (0-based).
*
* @return The MathNode tree for this term.
  */
MathNodePtr buildTerm(const core::Value& derivativeAtCenter,
                      const std::string& variable, const core::Value& center,
                      std::size_t degree) {
    MathNodePtr derivativeNode =
        std::make_unique<NumberNode>(derivativeAtCenter.str());

    if (degree == 0) {
        return derivativeNode;
    }

    MathNodePtr factorialNode = buildFactorialNode(degree);

    MathNodePtr coefficientNode = std::make_unique<BinaryNode>(
        BinaryOp::Divide, std::move(derivativeNode), std::move(factorialNode));

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

/**

* @brief Counts the number of nodes in a MathNode tree.
*
* Every MathNode contributes one node to the count, including the root.
*
* @param node Root of the tree to count.
*
* @return Total number of nodes in the tree.
  */
std::size_t countNodes(const MathNode& node) {
    return dispatch::Dispatcher::dispatch(
        node, [](const auto& concreteNode) -> std::size_t {
            using NodeType = std::decay_t<decltype(concreteNode)>;

            if constexpr (std::is_same_v<NodeType, NumberNode>) {
                return 1;

            } else if constexpr (std::is_same_v<NodeType, SymbolNode>) {
                return 1;

            } else if constexpr (std::is_same_v<NodeType, UnaryNode>) {
                return 1 + countNodes(*concreteNode.operand);

            } else if constexpr (std::is_same_v<NodeType, BinaryNode>) {
                return 1 + countNodes(*concreteNode.left) +
                       countNodes(*concreteNode.right);

            } else if constexpr (std::is_same_v<NodeType, FunctionNode>) {
                std::size_t count = 1;

                for (const auto& argument : concreteNode.arguments) {
                    count += countNodes(*argument);
                }

                return count;

            } else {
                static_assert(std::is_same_v<NodeType, void>,
                              "Taylor: unsupported MathNode type.");
            }
        });
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
                              "Taylor: unsupported MathNode type.");
            }
        });
}

}  // namespace

math::PreparedAst Taylor::series(const math::PreparedAst& prepared,
                                 const std::string& variable,
                                 const core::Value& center,
                                 const TaylorConfig& config) {
    // A single-symbol context: derivatives of `prepared` are evaluated at
    // `variable = center` to obtain the numerical value of each derivative.
    core::Context context;
    context.setValue(variable, center.str());

    // f(center) — the degree-0 term.
    core::Value valueAtCenter =
        numathap::backend::evaluate(prepared, context);

    MathNodePtr expansion =
        buildTerm(valueAtCenter, variable, center, 0);

    if (config.order > 0) {
        //
        // First derivative.
        //
        math::PreparedAst derivative =
            numathap::backend::differentiate::differentiate(
                prepared, variable);

        symbolic::UltraSimplifier ultraSimplifier;

        for (std::size_t degree = 1;
             degree <= config.order;
             ++degree) {
            //
            // Count the nodes of the current symbolic derivative.
            //
            const std::size_t derivativeNodes =
                countNodes(*derivative.root());

            //
            // Evaluate f^(degree)(center).
            //
            core::Value derivativeAtCenter =
                numathap::backend::evaluate(derivative, context);

            //
            // Add the current Taylor term.
            //
            expansion = std::make_unique<BinaryNode>(
                BinaryOp::Add,
                std::move(expansion),
                buildTerm(derivativeAtCenter,
                          variable,
                          center,
                          degree));

            //
            // The current derivative has already been used.
            // Do not generate the next derivative if the limit
            // has been reached.
            //
            if (derivativeNodes >= config.maxDerivativeNodes) {
                break;
            }

            //
            // Convert PreparedAst -> MathAst so that UltraSimplifier
            // can simplify the symbolic derivative tree.
            //
            math::MathAst derivativeAst(
                derivative.expression(),
                cloneNode(*derivative.root()));

            //
            // Reduce the current derivative before using it as the
            // input to the next symbolic differentiation.
            //
            auto simplifiedDerivativeAst =
                ultraSimplifier.simplify(derivativeAst);

            //
            // Convert MathAst -> PreparedAst.
            //
            derivative = math::PreparedAst(
                simplifiedDerivativeAst.expression(),
                cloneNode(*simplifiedDerivativeAst.root()),
                derivative.environment());

            //
            // Generate the next derivative from the simplified tree.
            //
            derivative =
                numathap::backend::differentiate::differentiate(
                    derivative, variable);
        }
    }

    //
    // Final simplification of the generated Taylor expression.
    //
    math::MathAst expansionAst(
        prepared.expression(),
        std::move(expansion));

    symbolic::Simplifier simplifier;
    auto simplifiedAst =
        simplifier.simplify(expansionAst);

    //
    // Clone the simplified tree because MathAst owns its root.
    //
    auto preparedRoot =
        cloneNode(*simplifiedAst.root());

    std::string expression =
        "taylor(" + prepared.expression() + ", " +
        variable + ", " + center.str() + ", " +
        std::to_string(config.order) + ")";

    return math::PreparedAst(
        std::move(expression),
        std::move(preparedRoot),
        prepared.environment());
}

}  // namespace numathap::backend::series

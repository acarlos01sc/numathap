/**
 * @file Differentiator.cpp
 * @brief Implements the Differentiator backend.
 */

#include "numathap/backend/differentiate/Differentiator.hpp"

#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "numathap/backend/differentiate/CMathDifferentiator.hpp"
#include "numathap/dispatch/Dispatcher.hpp"
#include "numathap/symbolic/ConstantFolder.hpp"
#include "numathap/symbolic/Simplifier.hpp"

namespace numathap::backend::differentiate {

using namespace numathap::math;

namespace {

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

MathNodePtr makeBinary(BinaryOp op, MathNodePtr left, MathNodePtr right) {
    return std::make_unique<BinaryNode>(op, std::move(left), std::move(right));
}

MathNodePtr makeNumber(std::string value) {
    return std::make_unique<NumberNode>(std::move(value));
}

}  // namespace

PreparedAst Differentiator::differentiate(const PreparedAst& ast,
                                          std::string_view variable) const {
    if (ast.empty()) {
        throw std::logic_error(
            "Differentiator: cannot differentiate an empty PreparedAst.");
    }

    if (variable.empty()) {
        throw std::invalid_argument(
            "Differentiator: differentiation variable cannot be empty.");
    }

    auto derivativeRoot = differentiateNode(*ast.root(), variable);

    MathAst derivativeAst(ast.expression(), std::move(derivativeRoot));

    //
    // Constant folding is the first optimization pass.
    //
    symbolic::ConstantFolder constantFolder;
    auto foldedAst = constantFolder.fold(derivativeAst);

    //
    // Simplification is mandatory in the differentiation pipeline.
    //
    symbolic::Simplifier simplifier;
    auto simplifiedAst = simplifier.simplify(foldedAst);

    //
    // The simplified tree is already prepared. Clone it because MathAst owns
    // its root and exposes it only through a const pointer.
    //
    auto preparedRoot = cloneNode(*simplifiedAst.root());

    auto expression = std::string("d/d") + std::string(variable) + "(" +
                      ast.expression() + ")";

    return PreparedAst(std::move(expression), std::move(preparedRoot),
                       ast.environment());
}

MathNodePtr Differentiator::differentiateNode(const MathNode& node,
                                              std::string_view variable) const {
    return dispatch::Dispatcher::dispatch(
        node, [this, variable](const auto& concreteNode) -> MathNodePtr {
            using NodeType = std::decay_t<decltype(concreteNode)>;

            if constexpr (std::is_same_v<NodeType, NumberNode>) {
                return differentiateNumber(concreteNode);

            } else if constexpr (std::is_same_v<NodeType, SymbolNode>) {
                return differentiateSymbol(concreteNode, variable);

            } else if constexpr (std::is_same_v<NodeType, UnaryNode>) {
                return differentiateUnary(concreteNode, variable);

            } else if constexpr (std::is_same_v<NodeType, BinaryNode>) {
                return differentiateBinary(concreteNode, variable);

            } else if constexpr (std::is_same_v<NodeType, FunctionNode>) {
                return differentiateFunction(concreteNode, variable);

            } else {
                static_assert(std::is_same_v<NodeType, void>,
                              "Differentiator: unsupported MathNode type.");
            }
        });
}

MathNodePtr Differentiator::differentiateNumber(const NumberNode&) const {
    return makeNumber("0");
}

MathNodePtr Differentiator::differentiateSymbol(
    const SymbolNode& node, std::string_view variable) const {
    if (node.name == variable) {
        return makeNumber("1");
    }

    return makeNumber("0");
}

MathNodePtr Differentiator::differentiateUnary(
    const UnaryNode& node, std::string_view variable) const {
    auto derivative = differentiateNode(*node.operand, variable);

    switch (node.op) {
        case UnaryOp::Plus:
            return derivative;

        case UnaryOp::Minus:
            return std::make_unique<UnaryNode>(UnaryOp::Minus,
                                               std::move(derivative));
    }

    throw std::logic_error("Differentiator: unknown unary operator.");
}

MathNodePtr Differentiator::differentiateBinary(
    const BinaryNode& node, std::string_view variable) const {
    switch (node.op) {
        case BinaryOp::Add: {
            auto left = differentiateNode(*node.left, variable);
            auto right = differentiateNode(*node.right, variable);

            return makeBinary(BinaryOp::Add, std::move(left), std::move(right));
        }

        case BinaryOp::Subtract: {
            auto left = differentiateNode(*node.left, variable);
            auto right = differentiateNode(*node.right, variable);

            return makeBinary(BinaryOp::Subtract, std::move(left),
                              std::move(right));
        }

        case BinaryOp::Multiply: {
            //
            // (u * v)' = u' * v + u * v'
            //
            auto leftDerivative = differentiateNode(*node.left, variable);

            auto rightDerivative = differentiateNode(*node.right, variable);

            auto left = cloneNode(*node.left);
            auto right = cloneNode(*node.right);

            auto first =
                makeBinary(BinaryOp::Multiply, std::move(leftDerivative),
                           std::move(right));

            auto second = makeBinary(BinaryOp::Multiply, std::move(left),
                                     std::move(rightDerivative));

            return makeBinary(BinaryOp::Add, std::move(first),
                              std::move(second));
        }

        case BinaryOp::Divide: {
            //
            //       u' * v - u * v'
            // (u/v)' = ---------------
            //              v^2
            //
            auto leftDerivative = differentiateNode(*node.left, variable);

            auto rightDerivative = differentiateNode(*node.right, variable);

            auto left = cloneNode(*node.left);
            auto right = cloneNode(*node.right);
            auto denominator = cloneNode(*node.right);

            auto first =
                makeBinary(BinaryOp::Multiply, std::move(leftDerivative),
                           std::move(right));

            auto second = makeBinary(BinaryOp::Multiply, std::move(left),
                                     std::move(rightDerivative));

            auto numerator = makeBinary(BinaryOp::Subtract, std::move(first),
                                        std::move(second));

            auto denominatorSquared = makeBinary(
                BinaryOp::Power, std::move(denominator), makeNumber("2"));

            return makeBinary(BinaryOp::Divide, std::move(numerator),
                              std::move(denominatorSquared));
        }

        case BinaryOp::Power:
            return differentiatePower(node, variable);
    }

    throw std::logic_error("Differentiator: unknown binary operator.");
}

MathNodePtr Differentiator::differentiatePower(
    const BinaryNode& node, std::string_view variable) const {
    auto baseDerivative = differentiateNode(*node.left, variable);
    auto exponentDerivative = differentiateNode(*node.right, variable);

    CMathDifferentiator differentiator;

    return differentiator.differentiate(node, std::move(baseDerivative),
                                        std::move(exponentDerivative));
}

MathNodePtr Differentiator::differentiateFunction(
    const FunctionNode& node, std::string_view variable) const {
    if (node.arguments.size() != 1) {
        throw std::invalid_argument("Differentiator: function '" + node.name +
                                    "' expects exactly one argument.");
    }

    auto argumentDerivative = differentiateNode(*node.arguments[0], variable);

    CMathDifferentiator differentiator;

    return differentiator.differentiate(node, std::move(argumentDerivative));
}

}  // namespace numathap::backend::differentiate
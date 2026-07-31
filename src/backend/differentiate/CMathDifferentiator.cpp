/**
 * @file CMathDifferentiator.cpp
 * @brief Implements the CMathDifferentiator backend.
 */

#include "numathap/backend/differentiate/CMathDifferentiator.hpp"

#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include "numathap/dispatch/Dispatcher.hpp"

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
                static_assert(
                    std::is_same_v<NodeType, void>,
                    "CMathDifferentiator: unsupported MathNode type.");
            }
        });
}

MathNodePtr makeFunction(std::string name, MathNodePtr argument) {
    std::vector<MathNodePtr> arguments;
    arguments.push_back(std::move(argument));

    return std::make_unique<FunctionNode>(std::move(name),
                                          std::move(arguments));
}

MathNodePtr makeBinary(BinaryOp op, MathNodePtr left, MathNodePtr right) {
    return std::make_unique<BinaryNode>(op, std::move(left), std::move(right));
}

MathNodePtr makeNumber(std::string value) {
    return std::make_unique<NumberNode>(std::move(value));
}

MathNodePtr makeUnaryMinus(MathNodePtr operand) {
    return std::make_unique<UnaryNode>(UnaryOp::Minus, std::move(operand));
}

}  // namespace

MathNodePtr CMathDifferentiator::differentiate(
    const FunctionNode& node, MathNodePtr argumentDerivative) const {
    if (node.arguments.size() != 1) {
        throw std::invalid_argument("CMathDifferentiator: function '" +
                                    node.name +
                                    "' expects exactly one argument.");
    }

    //
    // The argument itself must be cloned because the derivative tree is
    // independent from the input AST.
    //
    auto argument = cloneNode(*node.arguments[0]);

    if (node.name == "sin") {
        //
        // (sin(u))' = cos(u) * u'
        //
        auto cosine = makeFunction("cos", std::move(argument));

        return makeBinary(BinaryOp::Multiply, std::move(cosine),
                          std::move(argumentDerivative));
    }

    if (node.name == "cos") {
        //
        // (cos(u))' = -sin(u) * u'
        //
        auto sine = makeFunction("sin", std::move(argument));
        auto negativeSine = makeUnaryMinus(std::move(sine));

        return makeBinary(BinaryOp::Multiply, std::move(negativeSine),
                          std::move(argumentDerivative));
    }

    if (node.name == "tan") {
        //
        // (tan(u))' = u' / cos(u)^2
        //
        auto cosine = makeFunction("cos", std::move(argument));

        auto denominator =
            makeBinary(BinaryOp::Power, std::move(cosine), makeNumber("2"));

        return makeBinary(BinaryOp::Divide, std::move(argumentDerivative),
                          std::move(denominator));
    }

    if (node.name == "asin") {
        //
        // (asin(u))' = u' / sqrt(1 - u^2)
        //
        auto base = cloneNode(*node.arguments[0]);

        auto square =
            makeBinary(BinaryOp::Power, std::move(base), makeNumber("2"));

        auto radicand =
            makeBinary(BinaryOp::Subtract, makeNumber("1"), std::move(square));

        auto denominator = makeFunction("sqrt", std::move(radicand));

        return makeBinary(BinaryOp::Divide, std::move(argumentDerivative),
                          std::move(denominator));
    }

    if (node.name == "acos") {
        //
        // (acos(u))' = -u' / sqrt(1 - u^2)
        //
        auto base = cloneNode(*node.arguments[0]);

        auto square =
            makeBinary(BinaryOp::Power, std::move(base), makeNumber("2"));

        auto radicand =
            makeBinary(BinaryOp::Subtract, makeNumber("1"), std::move(square));

        auto denominator = makeFunction("sqrt", std::move(radicand));

        auto quotient =
            makeBinary(BinaryOp::Divide, std::move(argumentDerivative),
                       std::move(denominator));

        return makeUnaryMinus(std::move(quotient));
    }

    if (node.name == "atan") {
        //
        // (atan(u))' = u' / (1 + u^2)
        //
        auto base = cloneNode(*node.arguments[0]);

        auto square =
            makeBinary(BinaryOp::Power, std::move(base), makeNumber("2"));

        auto denominator =
            makeBinary(BinaryOp::Add, makeNumber("1"), std::move(square));

        return makeBinary(BinaryOp::Divide, std::move(argumentDerivative),
                          std::move(denominator));
    }

    if (node.name == "sinh") {
        //
        // (sinh(u))' = cosh(u) * u'
        //
        auto hyperbolicCosine = makeFunction("cosh", std::move(argument));

        return makeBinary(BinaryOp::Multiply, std::move(hyperbolicCosine),
                          std::move(argumentDerivative));
    }

    if (node.name == "cosh") {
        //
        // (cosh(u))' = sinh(u) * u'
        //
        auto hyperbolicSine = makeFunction("sinh", std::move(argument));

        return makeBinary(BinaryOp::Multiply, std::move(hyperbolicSine),
                          std::move(argumentDerivative));
    }

    if (node.name == "tanh") {
        //
        // (tanh(u))' = u' / cosh(u)^2
        //
        auto hyperbolicCosine = makeFunction("cosh", std::move(argument));

        auto denominator = makeBinary(
            BinaryOp::Power, std::move(hyperbolicCosine), makeNumber("2"));

        return makeBinary(BinaryOp::Divide, std::move(argumentDerivative),
                          std::move(denominator));
    }

    if (node.name == "exp") {
        //
        // (exp(u))' = exp(u) * u'
        //
        auto exponential = makeFunction("exp", std::move(argument));

        return makeBinary(BinaryOp::Multiply, std::move(exponential),
                          std::move(argumentDerivative));
    }

    if (node.name == "log") {
        //
        // (log(u))' = u' / u
        //
        return makeBinary(BinaryOp::Divide, std::move(argumentDerivative),
                          std::move(argument));
    }

    if (node.name == "sqrt") {
        //
        // (sqrt(u))' = u' / (2 * sqrt(u))
        //
        auto squareRoot = makeFunction("sqrt", std::move(argument));

        auto denominator = makeBinary(BinaryOp::Multiply, makeNumber("2"),
                                      std::move(squareRoot));

        return makeBinary(BinaryOp::Divide, std::move(argumentDerivative),
                          std::move(denominator));
    }

    if (node.name == "log10") {
        //
        // (log10(u))' = u' / (u * log(10))
        //
        auto logarithmOfTen = makeFunction("log", makeNumber("10"));

        auto denominator = makeBinary(BinaryOp::Multiply, std::move(argument),
                                      std::move(logarithmOfTen));

        return makeBinary(BinaryOp::Divide, std::move(argumentDerivative),
                          std::move(denominator));
    }

    if (node.name == "log2") {
        //
        // (log2(u))' = u' / (u * log(2))
        //
        auto logarithmOfTwo = makeFunction("log", makeNumber("2"));

        auto denominator = makeBinary(BinaryOp::Multiply, std::move(argument),
                                      std::move(logarithmOfTwo));

        return makeBinary(BinaryOp::Divide, std::move(argumentDerivative),
                          std::move(denominator));
    }

    if (node.name == "cbrt") {
        //
        // (cbrt(u))' = u' / (3 * cbrt(u)^2)
        //
        auto cubeRoot = makeFunction("cbrt", std::move(argument));

        auto square =
            makeBinary(BinaryOp::Power, std::move(cubeRoot), makeNumber("2"));

        auto denominator =
            makeBinary(BinaryOp::Multiply, makeNumber("3"), std::move(square));

        return makeBinary(BinaryOp::Divide, std::move(argumentDerivative),
                          std::move(denominator));
    }

    throw std::logic_error("CMathDifferentiator: function '" + node.name +
                           "' differentiation is not implemented.");
}

MathNodePtr CMathDifferentiator::differentiate(
    const BinaryNode& node, MathNodePtr leftDerivative,
    MathNodePtr rightDerivative) const {
    //
    // (u^v)' = v * u^(v - 1) * u'
    //         + u^v * log(u) * v'
    //

    //
    // First term:
    //
    // v * u^(v - 1) * u'
    //
    auto exponent = cloneNode(*node.right);

    auto exponentMinusOne =
        makeBinary(BinaryOp::Subtract, cloneNode(*node.right), makeNumber("1"));

    auto powerDerivative = makeBinary(BinaryOp::Power, cloneNode(*node.left),
                                      std::move(exponentMinusOne));

    auto firstFactor = makeBinary(BinaryOp::Multiply, std::move(exponent),
                                  std::move(powerDerivative));

    auto firstTerm = makeBinary(BinaryOp::Multiply, std::move(firstFactor),
                                std::move(leftDerivative));

    //
    // Second term:
    //
    // u^v * log(u) * v'
    //
    auto power = makeBinary(BinaryOp::Power, cloneNode(*node.left),
                            cloneNode(*node.right));

    auto logarithm = makeFunction("log", cloneNode(*node.left));

    auto secondFactor =
        makeBinary(BinaryOp::Multiply, std::move(power), std::move(logarithm));

    auto secondTerm = makeBinary(BinaryOp::Multiply, std::move(secondFactor),
                                 std::move(rightDerivative));

    return makeBinary(BinaryOp::Add, std::move(firstTerm),
                      std::move(secondTerm));
}

}  // namespace numathap::backend::differentiate
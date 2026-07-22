#include "numathap/symbolic/Simplifier.hpp"

#include <stdexcept>
#include <utility>
#include <vector>

namespace numathap::symbolic {

using namespace numathap::math;

MathAst
Simplifier::simplify(const MathAst& mathAst) const
{
    if (mathAst.root() == nullptr) {
        return MathAst(
            mathAst.expression(),
            nullptr);
    }

    return MathAst(
        mathAst.expression(),
        simplifyNode(*mathAst.root()));
}


MathNodePtr
Simplifier::simplifyNode(const MathNode& node) const
{
    if (auto p = dynamic_cast<const NumberNode*>(&node)) {
        return simplifyNumber(*p);
    }

    if (auto p = dynamic_cast<const SymbolNode*>(&node)) {
        return simplifySymbol(*p);
    }

    if (auto p = dynamic_cast<const UnaryNode*>(&node)) {
        return simplifyUnary(*p);
    }

    if (auto p = dynamic_cast<const BinaryNode*>(&node)) {
        return simplifyBinary(*p);
    }

    if (auto p = dynamic_cast<const FunctionNode*>(&node)) {
        return simplifyFunction(*p);
    }

    throw std::runtime_error("Unknown MathNode type.");
}


MathNodePtr
Simplifier::simplifyNumber(const NumberNode& node) const
{
    return std::make_unique<NumberNode>(node.value);
}


MathNodePtr
Simplifier::simplifySymbol(const SymbolNode& node) const
{
    return std::make_unique<SymbolNode>(node.name);
}


MathNodePtr
Simplifier::simplifyUnary(const UnaryNode& node) const
{
    auto operand =
        simplifyNode(*node.operand);

    return simplifyUnaryNode(
        node.op,
        std::move(operand));
}


MathNodePtr
Simplifier::simplifyBinary(const BinaryNode& node) const
{
    auto left =
        simplifyNode(*node.left);

    auto right =
        simplifyNode(*node.right);

    return simplifyBinaryNode(
        node.op,
        std::move(left),
        std::move(right));
}


MathNodePtr
Simplifier::simplifyFunction(const FunctionNode& node) const
{
    std::vector<MathNodePtr> arguments;

    arguments.reserve(node.arguments.size());

    for (const auto& argument : node.arguments) {
        arguments.push_back(
            simplifyNode(*argument));
    }

    return std::make_unique<FunctionNode>(
        node.name,
        std::move(arguments));
}


MathNodePtr
Simplifier::simplifyUnaryNode(
    UnaryOp op,
    MathNodePtr operand) const
{
    //
    // +x -> x
    //
    if (op == UnaryOp::Plus) {
        return operand;
    }

    //
    // -(-x) -> x
    //
    if (op == UnaryOp::Minus) {
        if (auto unary =
                dynamic_cast<UnaryNode*>(operand.get());
            unary != nullptr &&
            unary->op == UnaryOp::Minus)
        {
            return std::move(unary->operand);
        }
    }

    return std::make_unique<UnaryNode>(
        op,
        std::move(operand));
}


MathNodePtr
Simplifier::simplifyBinaryNode(
    BinaryOp op,
    MathNodePtr left,
    MathNodePtr right) const
{
    switch (op) {

    case BinaryOp::Add:

        if (isZero(*left))
            return right;

        if (isZero(*right))
            return left;

        break;


    case BinaryOp::Subtract:

        if (isZero(*right))
            return left;

        if (isZero(*left))
            return std::make_unique<UnaryNode>(
                UnaryOp::Minus,
                std::move(right));

        break;


    case BinaryOp::Multiply:

        if (isZero(*left))
            return left;

        if (isZero(*right))
            return right;

        if (isOne(*left))
            return right;

        if (isOne(*right))
            return left;

        if (isUnaryMinusOne(*left))
            return std::make_unique<UnaryNode>(
                UnaryOp::Minus,
                std::move(right));

        if (isUnaryMinusOne(*right))
            return std::make_unique<UnaryNode>(
                UnaryOp::Minus,
                std::move(left));

        break;


    case BinaryOp::Divide:

        if (isZero(*left))
            return left;

        if (isOne(*right))
            return left;

        break;


    default:
        break;
    }

    return std::make_unique<BinaryNode>(
        op,
        std::move(left),
        std::move(right));
}


bool
Simplifier::isZero(const MathNode& node) const
{
    auto number =
        dynamic_cast<const NumberNode*>(&node);

    return number != nullptr &&
           number->value == "0";
}


bool
Simplifier::isOne(const MathNode& node) const
{
    auto number =
        dynamic_cast<const NumberNode*>(&node);

    return number != nullptr &&
           number->value == "1";
}


bool
Simplifier::isUnaryMinusOne(const MathNode& node) const
{
    auto unary =
        dynamic_cast<const UnaryNode*>(&node);

    if (unary == nullptr ||
        unary->op != UnaryOp::Minus) {
        return false;
    }

    return isOne(*unary->operand);
}

} // namespace numathap::symbolic
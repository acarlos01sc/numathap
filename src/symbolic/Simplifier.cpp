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
    return std::make_unique<NumberNode>(
        node.value);
}


MathNodePtr
Simplifier::simplifySymbol(const SymbolNode& node) const
{
    return std::make_unique<SymbolNode>(
        node.name);
}


MathNodePtr
Simplifier::simplifyUnary(const UnaryNode& node) const
{
    auto operand =
        simplifyNode(*node.operand);

    return std::make_unique<UnaryNode>(
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

    return std::make_unique<BinaryNode>(
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

} // namespace numathap::symbolic
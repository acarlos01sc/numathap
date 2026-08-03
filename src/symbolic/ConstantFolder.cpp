#include "numathap/symbolic/ConstantFolder.hpp"

#include <stdexcept>
#include <utility>
#include <vector>

#include "numathap/core/Value.hpp"

namespace numathap::symbolic {

using namespace numathap::math;

MathAst ConstantFolder::fold(const MathAst& mathAst) const {
    if (mathAst.root() == nullptr) {
        return MathAst(mathAst.expression(), nullptr);
    }

    return MathAst(mathAst.expression(), foldNode(*mathAst.root()));
}

MathNodePtr ConstantFolder::foldNode(const MathNode& node) const {
    if (auto p = dynamic_cast<const NumberNode*>(&node)) {
        return foldNumber(*p);
    }

    if (auto p = dynamic_cast<const SymbolNode*>(&node)) {
        return foldSymbol(*p);
    }

    if (auto p = dynamic_cast<const UnaryNode*>(&node)) {
        return foldUnary(*p);
    }

    if (auto p = dynamic_cast<const BinaryNode*>(&node)) {
        return foldBinary(*p);
    }

    if (auto p = dynamic_cast<const FunctionNode*>(&node)) {
        return foldFunction(*p);
    }

    throw std::runtime_error("Unknown MathNode type.");
}

MathNodePtr ConstantFolder::foldNumber(const NumberNode& node) const {
    return std::make_unique<NumberNode>(node.value);
}

MathNodePtr ConstantFolder::foldSymbol(const SymbolNode& node) const {
    return std::make_unique<SymbolNode>(node.name);
}

MathNodePtr ConstantFolder::foldUnary(const UnaryNode& node) const {
    auto operand = foldNode(*node.operand);

    return foldUnaryNode(node.op, std::move(operand));
}

MathNodePtr ConstantFolder::foldBinary(const BinaryNode& node) const {
    auto left = foldNode(*node.left);

    auto right = foldNode(*node.right);

    return foldBinaryNode(node.op, std::move(left), std::move(right));
}

MathNodePtr ConstantFolder::foldFunction(const FunctionNode& node) const {
    std::vector<MathNodePtr> arguments;

    arguments.reserve(node.arguments.size());

    for (const auto& argument : node.arguments) {
        arguments.push_back(foldNode(*argument));
    }

    return std::make_unique<FunctionNode>(node.name, std::move(arguments));
}

MathNodePtr ConstantFolder::foldUnaryNode(UnaryOp op,
                                          MathNodePtr operand) const {
    if (!isNumber(*operand)) {
        return std::make_unique<UnaryNode>(op, std::move(operand));
    }

    core::Value value =
        core::Value::parse(static_cast<NumberNode*>(operand.get())->value);

    switch (op) {
        case UnaryOp::Plus:
            return std::make_unique<NumberNode>((+value).str());

        case UnaryOp::Minus:
            return std::make_unique<NumberNode>((-value).str());
    }

    return std::make_unique<UnaryNode>(op, std::move(operand));
}

MathNodePtr ConstantFolder::foldBinaryNode(BinaryOp op, MathNodePtr left,
                                           MathNodePtr right) const {
    if (!isNumber(*left) || !isNumber(*right)) {
        return std::make_unique<BinaryNode>(op, std::move(left),
                                            std::move(right));
    }

    core::Value lhs =
        core::Value::parse(static_cast<NumberNode*>(left.get())->value);

    core::Value rhs =
        core::Value::parse(static_cast<NumberNode*>(right.get())->value);

    switch (op) {
        case BinaryOp::Add:
            return std::make_unique<NumberNode>((lhs + rhs).str());

        case BinaryOp::Subtract:
            return std::make_unique<NumberNode>((lhs - rhs).str());

        case BinaryOp::Multiply:
            return std::make_unique<NumberNode>((lhs * rhs).str());

        case BinaryOp::Divide:
            return std::make_unique<NumberNode>((lhs / rhs).str());

        default:
            return std::make_unique<BinaryNode>(op, std::move(left),
                                                std::move(right));
    }
}

bool ConstantFolder::isNumber(const MathNode& node) const {
    return dynamic_cast<const NumberNode*>(&node) != nullptr;
}

}  // namespace numathap::symbolic
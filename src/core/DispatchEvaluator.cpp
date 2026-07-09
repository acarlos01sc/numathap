#include "numathap/core/DispatchEvaluator.hpp"

#include <stdexcept>

#include "numathap/ast/NodeDispatcher.hpp"

namespace numathap::core {

DispatchEvaluator::DispatchEvaluator(const Context& context) noexcept
    : context_(context)
{
}

Value DispatchEvaluator::evaluate(const ast::Node& node) const
{
    return ast::NodeDispatcher::visit(
        node,
        [this](const auto& n)
        {
            return visit(n);
        });
}

Value DispatchEvaluator::visit(const ast::NumberNode& node) const
{
    return Value(std::stod(node.value));
}

Value DispatchEvaluator::visit(const ast::IdentifierNode& node) const
{
    return context_.get(node.name);
}

Value DispatchEvaluator::visit(const ast::UnaryNode& node) const
{
    Value operand = evaluate(*node.operand);

    switch (node.op) {
        case ast::UnaryOp::Plus:
            return +operand;

        case ast::UnaryOp::Minus:
            return -operand;
    }

    throw std::logic_error("Unknown unary operator.");
}

Value DispatchEvaluator::visit(const ast::BinaryNode& node) const
{
    Value lhs = evaluate(*node.left);
    Value rhs = evaluate(*node.right);

    switch (node.op) {
        case ast::BinaryOp::Add:
            return lhs + rhs;

        case ast::BinaryOp::Subtract:
            return lhs - rhs;

        case ast::BinaryOp::Multiply:
            return lhs * rhs;

        case ast::BinaryOp::Divide:
            return lhs / rhs;

        case ast::BinaryOp::Power:
            throw std::runtime_error(
                "Power operator not implemented yet.");
    }

    throw std::logic_error("Unknown binary operator.");
}

Value DispatchEvaluator::visit(const ast::PostfixNode&) const
{
    throw std::runtime_error(
        "Postfix operators are not implemented yet.");
}

Value DispatchEvaluator::visit(const ast::FunctionCallNode& node) const
{
    //
    // Future implementation:
    //
    // std::vector<Value> arguments;
    //
    // for (const auto& argument : node.arguments) {
    //     arguments.push_back(evaluate(*argument));
    // }
    //
    // return functionRegistry_.invoke(node.name, arguments);
    //

    (void)node;

    throw std::runtime_error(
        "Function evaluation is not implemented yet.");
}

Value DispatchEvaluator::visit(const ast::AbsoluteNode&) const
{
    throw std::runtime_error(
        "Absolute value is not implemented yet.");
}

}  // namespace numathap::core
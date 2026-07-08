#include "numathap/core/Evaluator.hpp"

#include <cmath>
#include <stdexcept>

#include "numathap/ast/Node.hpp"

namespace numathap::core {

namespace {

//------------------------------------------------------------
// Forward declaration
//------------------------------------------------------------

Value evaluateNode(const ast::Node&, const Context&);

//------------------------------------------------------------
// Number
//------------------------------------------------------------

Value evaluateNumber(const ast::NumberNode& node)
{
    return Value(std::stod(node.value));
}

//------------------------------------------------------------
// Identifier
//------------------------------------------------------------

Value evaluateIdentifier(const ast::IdentifierNode& node,
                         const Context& context)
{
    return context.get(node.name);
}

//------------------------------------------------------------
// Unary
//------------------------------------------------------------

Value evaluateUnary(const ast::UnaryNode& node,
                    const Context& context)
{
    Value value = evaluateNode(*node.operand, context);

    switch (node.op)
    {
    case ast::UnaryOp::Plus:
        return value;

    case ast::UnaryOp::Minus:
        return Value(-value.real().value());
    }

    throw std::runtime_error("Unknown unary operator.");
}

//------------------------------------------------------------
// Binary
//------------------------------------------------------------

Value evaluateBinary(const ast::BinaryNode& node,
                     const Context& context)
{
    Value left = evaluateNode(*node.left, context);
    Value right = evaluateNode(*node.right, context);

    switch (node.op)
    {
    case ast::BinaryOp::Add:
        return left + right;

    case ast::BinaryOp::Subtract:
        return left - right;

    case ast::BinaryOp::Multiply:
        return left * right;

    case ast::BinaryOp::Divide:
        return left / right;

    case ast::BinaryOp::Power:
        return Value(std::pow(left.real().value(),
                              right.real().value()));
    }

    throw std::runtime_error("Unknown binary operator.");
}

//------------------------------------------------------------
// Factorial
//------------------------------------------------------------

Value evaluatePostfix(const ast::PostfixNode&,
                      const Context&)
{
    throw std::runtime_error("Factorial not implemented.");
}

//------------------------------------------------------------
// Absolute
//------------------------------------------------------------

Value evaluateAbsolute(const ast::AbsoluteNode& node,
                       const Context& context)
{
    Value value = evaluateNode(*node.operand, context);

    return Value(std::abs(value.real().value()));
}

//------------------------------------------------------------
// Function
//------------------------------------------------------------

Value evaluateFunction(const ast::FunctionCallNode&,
                       const Context&)
{
    throw std::runtime_error("Functions not implemented.");
}

//------------------------------------------------------------
// Dispatcher
//------------------------------------------------------------

Value evaluateNode(const ast::Node& node,
                   const Context& context)
{
    if (auto p = dynamic_cast<const ast::NumberNode*>(&node))
        return evaluateNumber(*p);

    if (auto p = dynamic_cast<const ast::IdentifierNode*>(&node))
        return evaluateIdentifier(*p, context);

    if (auto p = dynamic_cast<const ast::UnaryNode*>(&node))
        return evaluateUnary(*p, context);

    if (auto p = dynamic_cast<const ast::BinaryNode*>(&node))
        return evaluateBinary(*p, context);

    if (auto p = dynamic_cast<const ast::PostfixNode*>(&node))
        return evaluatePostfix(*p, context);

    if (auto p = dynamic_cast<const ast::FunctionCallNode*>(&node))
        return evaluateFunction(*p, context);

    if (auto p = dynamic_cast<const ast::AbsoluteNode*>(&node))
        return evaluateAbsolute(*p, context);

    throw std::runtime_error("Unknown AST node.");
}

} // namespace

//------------------------------------------------------------
// Evaluator
//------------------------------------------------------------

Evaluator::Evaluator(const Context& context) noexcept
    : context_(context)
{
}

Value Evaluator::evaluate(const ast::Node& node) const
{
    return evaluateNode(node, context_);
}

} // namespace numathap::core
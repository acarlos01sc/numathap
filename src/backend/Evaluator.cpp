#include "numathap/backend/Evaluator.hpp"

#include <stdexcept>
#include <vector>

#include "numathap/config/MathAdapter.hpp"
#include "numathap/dispatch/Dispatcher.hpp"
#include "numathap/numeric/Real.hpp"

namespace numathap::backend {

Evaluator::Evaluator(
    const math::PreparedAst& prepared,
    const core::Context& context)
    : prepared_(prepared),
      context_(context)
{
}

core::Value
Evaluator::evaluate(
    const math::PreparedAst& prepared,
    const core::Context& context)
{
    Evaluator evaluator(prepared, context);

    return dispatch::Dispatcher::dispatch(prepared, evaluator);
}

core::Value
Evaluator::dispatch(const math::MathNode& node) const
{
    return dispatch::Dispatcher::dispatchNode(node, *this);
}

core::Value
Evaluator::operator()(const math::NumberNode& node) const
{
    return parseValue(node.value);
}

core::Value
Evaluator::operator()(const math::SymbolNode& node) const
{
    return resolveSymbol(node.name);
}

core::Value
Evaluator::operator()(const math::UnaryNode& node) const
{
    const auto value = dispatch(*node.operand);

    switch (node.op) {
    case math::UnaryOp::Plus:
        return +value;

    case math::UnaryOp::Minus:
        return -value;
    }

    throw std::logic_error("Evaluator: unknown unary operator.");
}

core::Value
Evaluator::operator()(const math::BinaryNode& node) const
{
    const auto lhs = dispatch(*node.left);
    const auto rhs = dispatch(*node.right);

    switch (node.op) {

    case math::BinaryOp::Add:
        return lhs + rhs;

    case math::BinaryOp::Subtract:
        return lhs - rhs;

    case math::BinaryOp::Multiply:
        return lhs * rhs;

    case math::BinaryOp::Divide:
        return lhs / rhs;

    case math::BinaryOp::Power: {
        std::vector<core::Value> arguments;
        arguments.reserve(2);

        arguments.push_back(lhs);
        arguments.push_back(rhs);

        return prepared_.environment()
            .mathAdapter()
            .callFunction("pow", arguments);
    }
    }

    throw std::logic_error("Evaluator: unknown binary operator.");
}

core::Value
Evaluator::operator()(const math::FunctionNode& node) const
{
    std::vector<core::Value> arguments;
    arguments.reserve(node.arguments.size());

    for (const auto& arg : node.arguments) {
        arguments.push_back(dispatch(*arg));
    }

    return prepared_.environment()
        .mathAdapter()
        .callFunction(node.name, arguments);
}

core::Value
Evaluator::resolveSymbol(const std::string& symbol) const
{
    auto definition = context_.findValue(symbol);

    if (!definition) {
        return prepared_.environment()
            .mathAdapter()
            .resolveConstant(symbol);
    }

    return parseValue(*definition);
}

core::Value
Evaluator::parseValue(const std::string& text) const
{
    try {

        std::size_t pos = 0;

        const auto value = std::stod(text, &pos);

        if (pos == text.size()) {
            return core::Value(
                numeric::Real::Storage(value));
        }

    } catch (...) {
    }

    return prepared_.environment()
        .mathAdapter()
        .resolveConstant(text);
}

} // namespace numathap::backend
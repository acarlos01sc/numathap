#pragma once

#include <stdexcept>
#include <utility>

namespace numathap::dispatch {


template<typename Visitor>
decltype(auto)
Dispatcher::dispatch(
    const math::PreparedAst& ast,
    Visitor&& visitor)
{
    if (ast.empty()) {
        throw std::logic_error(
            "Dispatcher: cannot dispatch an empty PreparedAst.");
    }

    return dispatchNode(
        *ast.root(),
        std::forward<Visitor>(visitor));
}


template<typename Visitor>
decltype(auto)
Dispatcher::dispatchNode(
    const math::MathNode& node,
    Visitor&& visitor)
{
    using namespace math;

    if (const auto* n =
            dynamic_cast<const NumberNode*>(&node)) {
        return visitor(*n);
    }

    if (const auto* n =
            dynamic_cast<const SymbolNode*>(&node)) {
        return visitor(*n);
    }

    if (const auto* n =
            dynamic_cast<const UnaryNode*>(&node)) {
        return visitor(*n);
    }

    if (const auto* n =
            dynamic_cast<const BinaryNode*>(&node)) {
        return visitor(*n);
    }

    if (const auto* n =
            dynamic_cast<const FunctionNode*>(&node)) {
        return visitor(*n);
    }

    throw std::logic_error(
        "Dispatcher: unknown MathNode type.");
}


} // namespace numathap::dispatch
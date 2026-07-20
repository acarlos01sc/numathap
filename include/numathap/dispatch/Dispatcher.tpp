/**
 * @file Dispatcher.tpp
 * @brief Template implementation of the Dispatcher class methods.
 */
#pragma once

#include <stdexcept>
#include <utility>

namespace numathap::dispatch {

template <typename Visitor>
decltype(auto) Dispatcher::dispatch(const math::PreparedAst& ast,
                                    Visitor&& visitor) {
    // Ensure the tree structure is valid before initiating dispatch
    if (ast.empty()) {
        throw std::logic_error(
            "Dispatcher: cannot dispatch an empty PreparedAst.");
    }

    return dispatchNode(*ast.root(), std::forward<Visitor>(visitor));
}

template <typename Visitor>
decltype(auto) Dispatcher::dispatchNode(const math::MathNode& node,
                                        Visitor&& visitor) {
    using namespace math;

    // Type-dispatch mechanism using dynamic_cast
    // Note: The order of checks should ideally follow frequency of occurrence
    if (const auto* n = dynamic_cast<const NumberNode*>(&node)) {
        return visitor(*n);
    }

    if (const auto* n = dynamic_cast<const SymbolNode*>(&node)) {
        return visitor(*n);
    }

    if (const auto* n = dynamic_cast<const UnaryNode*>(&node)) {
        return visitor(*n);
    }

    if (const auto* n = dynamic_cast<const BinaryNode*>(&node)) {
        return visitor(*n);
    }

    if (const auto* n = dynamic_cast<const FunctionNode*>(&node)) {
        return visitor(*n);
    }

    // This branch is reached if a new MathNode subclass is added but
    // not included in the Dispatcher logic.
    throw std::logic_error("Dispatcher: unknown MathNode type.");
}

}  // namespace numathap::dispatch
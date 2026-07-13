#pragma once

#include <stdexcept>

namespace numathap::dispatch {

template<typename Visitor>
decltype(auto)
Dispatcher::dispatch(
    const math::MathNode& node,
    Visitor&& visitor)
{
    if (const auto* n =
            dynamic_cast<const math::NumberNode*>(&node)) {
        return visitor(*n);
    }

    if (const auto* n =
            dynamic_cast<const math::SymbolNode*>(&node)) {
        return visitor(*n);
    }

    if (const auto* n =
            dynamic_cast<const math::UnaryNode*>(&node)) {
        return visitor(*n);
    }

    if (const auto* n =
            dynamic_cast<const math::BinaryNode*>(&node)) {
        return visitor(*n);
    }

    if (const auto* n =
            dynamic_cast<const math::FunctionNode*>(&node)) {
        return visitor(*n);
    }

    throw std::logic_error("Unknown MathNode type.");
}

}  // namespace numathap::dispatch
/**
 * @file BackendSupport.cpp
 * @brief Implements shared backend utility services.
 */

#include "numathap/backend/BackendSupport.hpp"

#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include "numathap/backend/evaluate.hpp"
#include "numathap/dispatch/Dispatcher.hpp"
#include "numathap/math/prepare.hpp"
#include "numathap/numeric/Real.hpp"

namespace numathap::backend {

core::Value BackendSupport::evaluateConstant(
    const std::string& expression, const core::Context& context,
    const config::MathEnvironment& environment) {
    //
    // 1. Numeric literal
    //
    try {
        std::size_t pos = 0;
        const auto value = std::stod(expression, &pos);
        if (pos == expression.size()) {
            return core::Value(numeric::Real::Storage(value));
        }
    } catch (...) {
    }

    //
    // 2. Mathematical constant
    //
    try {
        return environment.mathAdapter().resolveConstant(expression);
    } catch (...) {
    }

    //
    // 3. Constant expression
    //
    try {
        auto prepared = math::prepare(expression, environment);
        return backend::evaluate(prepared, context);
    } catch (...) {
        throw std::invalid_argument(
            "Invalid numeric value or constant expression: \"" + expression +
            "\"");
    }
}

math::MathNodePtr BackendSupport::cloneNode(const math::MathNode& node) {
    using namespace numathap::math;

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
                    "BackendSupport::cloneNode: unsupported MathNode type.");
            }
        });
}

}  // namespace numathap::backend
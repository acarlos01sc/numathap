#include "numathap/backend/Evaluator.hpp"

#include <array>
#include <span>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include "numathap/dispatch/Dispatcher.hpp"

namespace numathap::backend {

Evaluator::Evaluator(const config::MathAdapter& adapter) : adapter_(adapter) {}

void Evaluator::prepare(math::MathNodePtr preparedAst) {
    preparedAst_ = std::move(preparedAst);
}

core::Value Evaluator::calc(const core::Context& context) const {
    if (!preparedAst_) {
        throw std::logic_error("Evaluator has no prepared AST.");
    }

    return evaluateNode(*preparedAst_, context);
}

core::Value Evaluator::evaluateNode(const math::MathNode& node,
                                    const core::Context& context) const {
    return dispatch::Dispatcher::dispatch(
        node, [&](const auto& current) -> core::Value {
            using NodeType = std::decay_t<decltype(current)>;

            //--------------------------------------------------
            // Number literal
            //--------------------------------------------------

            if constexpr (std::is_same_v<NodeType, math::NumberNode>) {
                return core::Value(
                    numeric::Real::Storage(std::stod(current.value)));
            }

            //--------------------------------------------------
            // Symbol
            //--------------------------------------------------

            else if constexpr (std::is_same_v<NodeType, math::SymbolNode>) {
                /*
                 * Symbol resolution order:
                 *
                 * 1) User variables
                 * 2) Mathematical constants
                 *
                 */

                if (context.contains(current.name)) {
                    return context.get(current.name);
                }

                return adapter_.resolveConstant(current.name);
            }

            //--------------------------------------------------
            // Unary operators
            //--------------------------------------------------

            else if constexpr (std::is_same_v<NodeType, math::UnaryNode>) {
                auto operand = evaluateNode(*current.operand, context);

                switch (current.op) {
                    case math::UnaryOp::Plus:
                        return operand;

                    case math::UnaryOp::Minus:
                        return -operand;
                }

                throw std::logic_error("Unknown unary operator.");
            }

            //--------------------------------------------------
            // Binary operators
            //--------------------------------------------------

            else if constexpr (std::is_same_v<NodeType, math::BinaryNode>) {
                auto left = evaluateNode(*current.left, context);

                auto right = evaluateNode(*current.right, context);

                switch (current.op) {
                    case math::BinaryOp::Add:
                        return left + right;

                    case math::BinaryOp::Subtract:
                        return left - right;

                    case math::BinaryOp::Multiply:
                        return left * right;

                    case math::BinaryOp::Divide:
                        return left / right;

                    case math::BinaryOp::Power: {
                        std::array<core::Value, 2> arguments{left, right};

                        return adapter_.callFunction(
                            "pow", std::span<const core::Value>(arguments));
                    }
                }

                throw std::logic_error("Unknown binary operator.");
            }

            //--------------------------------------------------
            // Function call
            //--------------------------------------------------

            else if constexpr (std::is_same_v<NodeType, math::FunctionNode>) {
                std::vector<core::Value> arguments;

                arguments.reserve(current.arguments.size());

                for (const auto& argument : current.arguments) {
                    arguments.push_back(evaluateNode(*argument, context));
                }

                return adapter_.callFunction(current.name, arguments);
            }

            //--------------------------------------------------
            // Unsupported node
            //--------------------------------------------------

            else {
                throw std::logic_error("Unsupported MathNode type.");
            }
        });
}

}  // namespace numathap::backend
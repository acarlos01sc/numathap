#pragma once

#include <stdexcept>
#include <utility>

#include "Node.hpp"

namespace numathap::ast {

/**
 * @brief Dispatches AST nodes according to their concrete type.
 *
 * NodeDispatcher centralizes all RTTI-based dispatching used by
 * algorithms operating on the Abstract Syntax Tree (AST).
 *
 * Instead of duplicating long sequences of dynamic_cast in classes
 * such as Evaluator, AstPrinter, Simplifier or Differentiator,
 * clients simply call visit() with a generic callable.
 *
 * Example:
 *
 * @code
 * return NodeDispatcher::visit(
 *     node,
 *     [this](const auto& n)
 *     {
 *         return evaluateNode(n);
 *     });
 * @endcode
 */
class NodeDispatcher {
   public:
    /**
     * @brief Visits an AST node according to its dynamic type.
     *
     * The visitor must be callable with every concrete node type.
     *
     * @tparam Visitor Callable object.
     * @param node AST node.
     * @param visitor Visitor callable.
     * @return Whatever the visitor returns.
     *
     * @throws std::logic_error if an unknown node type is encountered.
     */
    template <typename Visitor>
    static decltype(auto) visit(const Node& node, Visitor&& visitor)
    {
        if (const auto* p = dynamic_cast<const NumberNode*>(&node)) {
            return std::forward<Visitor>(visitor)(*p);
        }

        if (const auto* p = dynamic_cast<const IdentifierNode*>(&node)) {
            return std::forward<Visitor>(visitor)(*p);
        }

        if (const auto* p = dynamic_cast<const UnaryNode*>(&node)) {
            return std::forward<Visitor>(visitor)(*p);
        }

        if (const auto* p = dynamic_cast<const BinaryNode*>(&node)) {
            return std::forward<Visitor>(visitor)(*p);
        }

        if (const auto* p = dynamic_cast<const PostfixNode*>(&node)) {
            return std::forward<Visitor>(visitor)(*p);
        }

        if (const auto* p = dynamic_cast<const FunctionCallNode*>(&node)) {
            return std::forward<Visitor>(visitor)(*p);
        }

        if (const auto* p = dynamic_cast<const AbsoluteNode*>(&node)) {
            return std::forward<Visitor>(visitor)(*p);
        }

        throw std::logic_error(
            "NodeDispatcher: unsupported AST node type.");
    }
};

}  // namespace numathap::ast
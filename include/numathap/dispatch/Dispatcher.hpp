/**
 * @file Dispatcher.hpp
 * @brief Defines the Dispatcher class for visitor-based AST traversal.
 */
#pragma once

#include "numathap/math/MathNode.hpp"
#include "numathap/math/PreparedAst.hpp"

namespace numathap::backend {
class Evaluator;
}

namespace numathap::dispatch {

/**
 * @brief Dispatches nodes from a PreparedAst to a backend visitor.
 *
 * The Dispatcher acts as a central hub for type-erasure recovery. It inspects
 * the concrete type of a @ref math::MathNode and invokes the appropriate
 * overload on the provided Visitor.
 *
 * @note This class does not perform mathematical operations or tree traversal.
 * It is the responsibility of the Visitor (the backend) to implement recursion
 * for complex nodes (e.g., UnaryNode, BinaryNode).
 */
class Dispatcher {
   public:
    /**
     * @brief Dispatches the root node of an AST to the visitor.
     *
     * @tparam Visitor A callable type that must provide overloads for all
     *         concrete MathNode types (NumberNode, SymbolNode, etc.).
     * @param ast The PreparedAst containing the expression tree.
     * @param visitor The backend visitor instance.
     * @return The result of the visitor's call.
     * @throw std::logic_error If the AST is empty.
     */
    template <typename Visitor>
    static decltype(auto) dispatch(const math::PreparedAst& ast,
                                   Visitor&& visitor);

   private:
    friend class numathap::backend::Evaluator;

    /**
     * @brief Dispatches a concrete MathNode by performing type identification.
     *
     * This method attempts to cast the generic MathNode to its concrete type
     * and invokes the visitor.
     *
     * @tparam Visitor A callable type.
     * @param node The node to dispatch.
     * @param visitor The visitor to invoke.
     * @return The result of the visitor's call.
     * @throw std::logic_error If the node type is unrecognized or unsupported.
     */
    template <typename Visitor>
    static decltype(auto) dispatchNode(const math::MathNode& node,
                                       Visitor&& visitor);
};

}  // namespace numathap::dispatch

#include "Dispatcher.tpp"
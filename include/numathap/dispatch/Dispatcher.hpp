#pragma once

#include "numathap/math/PreparedAst.hpp"
#include "numathap/math/MathNode.hpp"

namespace numathap::backend {
class Evaluator;
}

namespace numathap::dispatch {

/**
 * @brief Dispatches nodes from a PreparedAst to a backend visitor.
 *
 * The Dispatcher does not perform mathematical operations,
 * does not modify the tree, and does not control recursion.
 *
 * Backend implementations are responsible for traversing child nodes.
 */
class Dispatcher {
public:

    /**
     * @brief Starts dispatching from a PreparedAst.
     *
     * The PreparedAst is the only public entry point.
     */
    template<typename Visitor>
    static decltype(auto) dispatch(
        const math::PreparedAst& ast,
        Visitor&& visitor);


private:

    friend class numathap::backend::Evaluator;

    /**
     * @brief Dispatches a concrete MathNode.
     *
     * Used internally and by backend recursion through the public
     * dispatch overload.
     */
    template<typename Visitor>
    static decltype(auto) dispatchNode(
        const math::MathNode& node,
        Visitor&& visitor);

};

} // namespace numathap::dispatch

#include "Dispatcher.tpp"
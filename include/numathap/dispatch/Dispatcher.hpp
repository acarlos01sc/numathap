#pragma once

#include "numathap/math/MathNode.hpp"

namespace numathap::dispatch {

/**
 * @brief Dispatches operations according to the concrete MathNode type.
 *
 * The Dispatcher centralizes runtime type inspection of the Prepared-AST.
 * It performs no mathematical computation and never modifies the tree.
 */
class Dispatcher {
   public:
    /**
     * @brief Dispatches a MathNode to a visitor.
     *
     * The visitor must implement operator() for every concrete MathNode type.
     */
    template<typename Visitor>
    static decltype(auto) dispatch(
        const math::MathNode& node,
        Visitor&& visitor);
};

}  // namespace numathap::dispatch

#include "Dispatcher.tpp"
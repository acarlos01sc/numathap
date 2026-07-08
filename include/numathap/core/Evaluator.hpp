#pragma once

#include "numathap/ast/Node.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"

namespace numathap::core {

/**
 * @brief Evaluates mathematical expression trees.
 *
 * An Evaluator traverses an Abstract Syntax Tree (AST) and computes its
 * numeric value using the variables defined in a Context.
 *
 * The evaluator does not own the Context. Variable lookup is performed
 * through the Context supplied at construction.
 */
class Evaluator {
   public:
    /**
     * @brief Creates an evaluator using the given context.
     *
     * @param context Variable lookup context.
     */
    explicit Evaluator(const Context& context) noexcept;

    /**
     * @brief Evaluates an expression tree.
     *
     * @param node Root node of the AST.
     * @return Computed value.
     */
    [[nodiscard]]
    Value evaluate(const ast::Node& node) const;

   private:
    const Context& context_;
};

}  // namespace numathap::core
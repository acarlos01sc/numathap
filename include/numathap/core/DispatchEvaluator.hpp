#pragma once

#include "numathap/ast/Node.hpp"
#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"

namespace numathap::core {

/**
 * @brief Evaluates mathematical expression trees using NodeDispatcher.
 *
 * DispatchEvaluator traverses an Abstract Syntax Tree (AST) by relying
 * on ast::NodeDispatcher to dispatch each node according to its concrete
 * type.
 *
 * The evaluator does not own the Context. Variable lookup is performed
 * through the Context supplied at construction.
 */
class DispatchEvaluator {
   public:
    /**
     * @brief Creates an evaluator using the given context.
     *
     * @param context Variable lookup context.
     */
    explicit DispatchEvaluator(const Context& context) noexcept;

    /**
     * @brief Evaluates an expression tree.
     *
     * @param node Root node of the AST.
     * @return Computed value.
     */
    [[nodiscard]]
    Value evaluate(const ast::Node& node) const;

   private:
    /**
     * @brief Evaluates a numeric literal.
     */
    [[nodiscard]]
    Value visit(const ast::NumberNode& node) const;

    /**
     * @brief Evaluates an identifier.
     */
    [[nodiscard]]
    Value visit(const ast::IdentifierNode& node) const;

    /**
     * @brief Evaluates a unary expression.
     */
    [[nodiscard]]
    Value visit(const ast::UnaryNode& node) const;

    /**
     * @brief Evaluates a binary expression.
     */
    [[nodiscard]]
    Value visit(const ast::BinaryNode& node) const;

    /**
     * @brief Evaluates a postfix expression.
     */
    [[nodiscard]]
    Value visit(const ast::PostfixNode& node) const;

    /**
     * @brief Evaluates a function call.
     */
    [[nodiscard]]
    Value visit(const ast::FunctionCallNode& node) const;

    /**
     * @brief Evaluates an absolute value expression.
     */
    [[nodiscard]]
    Value visit(const ast::AbsoluteNode& node) const;

   private:
    /// Variable lookup context.
    const Context& context_;
};

}  // namespace numathap::core
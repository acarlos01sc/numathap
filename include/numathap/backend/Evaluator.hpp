/**
 * @file Evaluator.hpp
 * @brief Defines the Evaluator class responsible for AST traversal and
 * numerical computation.
 */
#pragma once

#include "numathap/core/Context.hpp"
#include "numathap/core/Value.hpp"
#include "numathap/math/MathNode.hpp"
#include "numathap/math/PreparedAst.hpp"

namespace numathap::backend {

/**
 * @brief Stateless numerical evaluator implementing the Visitor pattern.
 *
 * The Evaluator traverses a @ref math::PreparedAst to compute the final
 * numerical result. It acts as a visitor for the @ref
 * numathap::dispatch::Dispatcher, processing each node type to derive a @ref
 * core::Value.
 *
 * @note This class maintains internal references to the AST and Context
 * during the dispatch process, so it should not outlive the objects passed to
 * it.
 */
class Evaluator {
   public:
    /**
     * @brief Evaluates a prepared mathematical expression.
     *
     * @param prepared The AST expression to evaluate.
     * @param context The execution context for symbol lookups and environment
     * settings.
     * @return The computed result as a @ref core::Value.
     */
    [[nodiscard]]
    static core::Value evaluate(const math::PreparedAst& prepared,
                                const core::Context& context);

    /** @name Dispatcher Visitors
     *  Overloaded operators that process specific MathNode types.
     */
    ///@{
    [[nodiscard]]
    core::Value operator()(const math::NumberNode& node) const;
    [[nodiscard]]
    core::Value operator()(const math::SymbolNode& node) const;
    [[nodiscard]]
    core::Value operator()(const math::UnaryNode& node) const;
    [[nodiscard]]
    core::Value operator()(const math::BinaryNode& node) const;
    [[nodiscard]]
    core::Value operator()(const math::FunctionNode& node) const;
    ///@}

   private:
    /**
     * @brief Private constructor used by the static evaluate() method.
     */
    Evaluator(const math::PreparedAst& prepared, const core::Context& context);

    /** @brief Dispatches a node to the appropriate visitor overload. */
    [[nodiscard]]
    core::Value dispatch(const math::MathNode& node) const;

    /** @brief Resolves a symbol's value using the current evaluation context.
     */
    [[nodiscard]]
    core::Value resolveSymbol(const std::string& symbol) const;

    /** @brief Parses a string definition into a @ref core::Value. */
    [[nodiscard]]
    core::Value parseValue(const std::string& text) const;

    /** @brief Evaluates an expression defined as a constant string. */
    [[nodiscard]]
    core::Value evaluateConstantExpression(const std::string& expression) const;

   private:
    const math::PreparedAst&
        prepared_;                  ///< Reference to the AST being evaluated.
    const core::Context& context_;  ///< Reference to the execution context.
};

}  // namespace numathap::backend
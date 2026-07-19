/**
 * @file MathAstBuilder.hpp
 * @brief Definition of the MathAstBuilder class.
 *
 * This file provides the logic for transforming a syntactic Parser-AST
 * into a semantic Mathematical AST (Math-AST).
 */
#pragma once

#include "numathap/math/MathNode.hpp"
#include "numathap/parser/ParserNode.hpp"

namespace numathap::math {

/**
 * @brief Builds a mathematical AST from a parser AST.
 *
 * The MathAstBuilder converts the syntactic representation produced by
 * the parser into a semantic representation suitable for mathematical
 * processing.
 *
 * The resulting Math-AST is independent of the parser grammar and is
 * the input consumed by subsequent stages such as the Orchestrator and
 * Dispatcher.
 */
class MathAstBuilder {
   public:
    /**
     * @brief Builds a complete Math-AST starting from a Parser-AST root.
     *
     * @param root The root node of the input Parser-AST.
     * @return A unique pointer to the root node of the generated Math-AST.
     */
    [[nodiscard]]
    MathNodePtr build(const parser::Node* root) const;

   private:
    /**
     * @brief Recursively converts a single Parser-AST node into its
     * corresponding Math-AST semantic node.
     *
     * @param node The Parser-AST node to transform.
     * @return A unique pointer to the corresponding Math-AST node.
     */
    [[nodiscard]]
    MathNodePtr buildNode(const parser::Node& node) const;
};

}  // namespace numathap::math
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
     * @brief Builds a Math-AST from a Parser-AST.
     *
     * @param root Root node of the Parser-AST.
     *
     * @return Root node of the generated Math-AST.
     */
    [[nodiscard]]
    MathNodePtr build(const parser::Node* root) const;

   private:
    /**
     * @brief Recursively converts a Parser-AST node into a Math-AST node.
     *
     * @param node Parser-AST node.
     *
     * @return Corresponding Math-AST node.
     */
    [[nodiscard]]
    MathNodePtr buildNode(const parser::Node& node) const;
};

}  // namespace numathap::math
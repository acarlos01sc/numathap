#pragma once

#include <ostream>
#include <string>

#include "numathap/math/MathNode.hpp"
#include "numathap/parser/ParserAst.hpp"

namespace numathap::math {

/**
 * @brief Semantic mathematical Abstract Syntax Tree.
 *
 * MathAst converts a Parser-AST into a semantic representation
 * independent of the parser grammar.
 *
 * It is the canonical input consumed by the Orchestrator.
 */
class MathAst {
   public:
    /**
     * @brief Builds the Math-AST from a Parser-AST.
     */
    explicit MathAst(const parser::ParserAst& parserAst);

    MathAst(MathAst&&) noexcept = default;
    MathAst& operator=(MathAst&&) noexcept = default;

    MathAst(const MathAst&) = delete;
    MathAst& operator=(const MathAst&) = delete;

    ~MathAst() = default;

    /**
     * @brief Returns the root node.
     */
    [[nodiscard]]
    const MathNode* root() const noexcept;

    /**
     * @brief Returns the original expression.
     */
    [[nodiscard]]
    const std::string& expression() const noexcept;

    /**
     * @brief Checks whether the tree is empty.
     */
    [[nodiscard]]
    bool empty() const noexcept;

    /**
     * @brief Prints the Math-AST.
     */
    void print(std::ostream& os) const;

   private:
    std::string expression_;
    MathNodePtr root_;
};

} // namespace numathap::math
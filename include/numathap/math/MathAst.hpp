/**
 * @file MathAst.hpp
 * @brief Definition of the MathAst class.
 *
 * This file provides the semantic representation of a mathematical expression,
 * abstracting the syntactic structure into a canonical tree ready for
 * orchestration.
 */
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
 * It acts as the canonical input consumed by the Orchestrator stage.
 */
class MathAst {
   public:
    /**
     * @brief Constructs a Math-AST from a Parser-AST.
     *
     * @param parserAst The source Parser-AST to be converted.
     */
    explicit MathAst(const parser::ParserAst& parserAst);

    MathAst(std::string expression, MathNodePtr root);

    MathAst(MathAst&&) noexcept = default;
    MathAst& operator=(MathAst&&) noexcept = default;

    MathAst(const MathAst&) = delete;
    MathAst& operator=(const MathAst&) = delete;

    ~MathAst() = default;

    /**
     * @brief Gets the root node of the Math-AST.
     * @return A const pointer to the root node.
     */
    [[nodiscard]]
    const MathNode* root() const noexcept;

    /**
     * @brief Gets the original mathematical expression string.
     * @return A const reference to the original expression.
     */
    [[nodiscard]]
    const std::string& expression() const noexcept;

    /**
     * @brief Checks whether the AST is empty.
     * @return true if the tree has no root node, false otherwise.
     */
    [[nodiscard]]
    bool empty() const noexcept;

    /**
     * @brief Prints the Math-AST to the specified output stream.
     * @param os The output stream to write to.
     */
    void print(std::ostream& os) const;

   private:
    /// @brief The original expression source text.
    std::string expression_;

    /// @brief The root node of the semantic tree.
    MathNodePtr root_;
};

}  // namespace numathap::math
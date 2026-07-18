/**
 * @file ParserAst.hpp
 * @brief Facade interface for processing and representing Abstract Syntax Trees (AST).
 * @see Parser.hpp
 * @see Lexer.hpp
 */
#pragma once

#include <ostream>
#include <string>

#include "ParserNode.hpp"

namespace numathap::parser {

/**
 * @brief Encapsulates the parser stage of expression processing.
 *
 * ParserAst converts a textual mathematical expression into a
 * syntactic Abstract Syntax Tree.
 *
 * Lexer and Parser details are hidden behind this interface.
 */
class ParserAst {
   public:
    explicit ParserAst(const std::string& expression);

    ParserAst(ParserAst&&) noexcept = default;
    ParserAst& operator=(ParserAst&&) noexcept = default;

    ParserAst(const ParserAst&) = delete;
    ParserAst& operator=(const ParserAst&) = delete;

    ~ParserAst() = default;

    /**
     * @brief Returns the root node of the syntax tree.
     */
    [[nodiscard]]
    const Node* root() const noexcept;

    /**
     * @brief Returns the string expression.
     */
    [[nodiscard]]
    const std::string& expression() const noexcept;

    /**
     * @brief Checks whether the AST is empty.
     */
    [[nodiscard]]
    bool empty() const noexcept;

    /**
     * @brief Displays the syntax tree.
     */
    void print(std::ostream& os) const;

   private:
    std::string expression_;
    NodePtr root_;
};

}  // namespace numathap::parser
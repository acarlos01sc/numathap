#pragma once

#include <vector>

#include "Lexer.hpp"
#include "Token.hpp"
#include "numathap/parser/ast/Node.hpp"

namespace numathap::parser {
/**
 * @brief Recursive-descent parser for mathematical expressions.
 *
 * The parser consumes tokens produced by the Lexer and builds an
 * Abstract Syntax Tree (AST) representing the input expression.
 *
 * Operator precedence and associativity are handled through the
 * recursive structure of the parsing functions.
 */
class Parser {
   public:
    explicit Parser(Lexer& lexer);

    [[nodiscard]]
    ast::NodePtr parse();

   private:
    Lexer& lexer_;
    Token current_;

    // Token handling
    void advance();
    bool match(TokenType type);
    bool check(TokenType type) const;
    void expect(TokenType type, const char* message);
    bool isAtEnd() const;

    // Grammar
    ast::NodePtr parseExpression();
    ast::NodePtr parseAdditive();
    ast::NodePtr parseMultiplicative();
    ast::NodePtr parsePower();
    ast::NodePtr parseUnary();
    ast::NodePtr parseExponent();
    ast::NodePtr parsePostfix();
    ast::NodePtr parsePrimary();

    // Primary expressions
    ast::NodePtr parseNumber();
    ast::NodePtr parseIdentifier();
    std::vector<ast::NodePtr> parseArguments();
    ast::NodePtr parseParenExpression();
    ast::NodePtr parseAbsolute();
};

}  // namespace numathap::parser

#pragma once

#include <vector>

#include "Lexer.hpp"
#include "Token.hpp"
#include "ParserNode.hpp"

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
    NodePtr parse();

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
    NodePtr parseExpression();
    NodePtr parseAdditive();
    NodePtr parseMultiplicative();
    NodePtr parsePower();
    NodePtr parseUnary();
    NodePtr parseExponent();
    NodePtr parsePostfix();
    NodePtr parsePrimary();

    // Primary expressions
    NodePtr parseNumber();
    NodePtr parseIdentifier();
    std::vector<NodePtr> parseArguments();
    NodePtr parseParenExpression();
    NodePtr parseAbsolute();
};

}  // namespace numathap::parser

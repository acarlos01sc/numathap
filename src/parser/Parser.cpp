#include "numathap/parser/Parser.hpp"

#include <memory>
#include <stdexcept>
#include <vector>

#include "numathap/parser/ast/Node.hpp"
#include "numathap/parser/Token.hpp"

namespace numathap::parser {

/**
 * @file Parser.cpp
 * @brief Recursive-descent parser implementation.
 */
Parser::Parser(Lexer& lexer) : lexer_(lexer) {
    // Initialize the parser with the first token.
    advance();
}

//--------------------------------------------------------------
// Token handling
//--------------------------------------------------------------

void Parser::advance() { current_ = lexer_.next(); }

bool Parser::check(TokenType type) const { return current_.type == type; }

bool Parser::match(TokenType type) {
    if (!check(type)) return false;

    advance();
    return true;
}

void Parser::expect(TokenType type, const char* message) {
    if (!match(type)) throw std::logic_error(message);
}

bool Parser::isAtEnd() const { return current_.type == TokenType::EndOfInput; }

//--------------------------------------------------------------
// Public interface
//--------------------------------------------------------------

/**
 * @brief Parses a complete expression.
 *
 * This is the public entry point of the parser. It parses a complete
 * mathematical expression and verifies that no unexpected tokens remain.
 *
 * Examples:
 *
 *     2 + 3 * x
 *     sin(x)^2
 *     (a+b)!
 *
 * @return The AST node representing the complete expression.
 *
 * @throws std::logic_error if unexpected tokens are found after the
 *         expression.
 */
ast::NodePtr Parser::parse() {
    auto expression = parseExpression();

    if (!isAtEnd()) {
        throw std::logic_error("Unexpected token after expression.");
    }

    return expression;
}

//--------------------------------------------------------------
// Grammar
//--------------------------------------------------------------

ast::NodePtr Parser::parseExpression() { return parseAdditive(); }

ast::NodePtr Parser::parseAdditive() {
    auto left = parseMultiplicative();

    while (true) {
        ast::BinaryOp op;

        if (match(TokenType::Plus)) {
            op = ast::BinaryOp::Add;
        } else if (match(TokenType::Minus)) {
            op = ast::BinaryOp::Subtract;
        } else {
            break;
        }

        auto right = parseMultiplicative();

        left = std::make_unique<ast::BinaryNode>(op, std::move(left),
                                                 std::move(right));
    }

    return left;
}

ast::NodePtr Parser::parseMultiplicative() {
    auto left = parseUnary();

    while (true) {
        ast::BinaryOp op;

        if (match(TokenType::Star)) {
            op = ast::BinaryOp::Multiply;
        } else if (match(TokenType::Slash)) {
            op = ast::BinaryOp::Divide;
        } else {
            break;
        }

        auto right = parseUnary();

        left = std::make_unique<ast::BinaryNode>(op, std::move(left),
                                                 std::move(right));
    }

    return left;
}

/**
 * @brief Parses power expressions.
 *
 * Power expressions are right-associative:
 *
 *     a^b^c == a^(b^c)
 *
 * The left operand is parsed as a postfix expression, allowing
 * constructs such as:
 *
 *     3!^2 == (3!)^2
 *
 * The exponent is parsed separately to allow signed exponents:
 *
 *     2^-3 == 2^(-3)
 *
 * @return The AST node representing a power expression.
 */
ast::NodePtr Parser::parsePower() {
    auto left = parsePostfix();

    if (!match(TokenType::Caret)) {
        return left;
    }

    auto right = parseExponent();

    return std::make_unique<ast::BinaryNode>(ast::BinaryOp::Power,
                                             std::move(left), std::move(right));
}

/**
 * @brief Parses an exponent expression.
 *
 * The exponent is parsed as a unary expression to allow signed
 * exponents such as 2^-3 while preserving the precedence rule
 * that unary operators have lower precedence than power:
 *
 *     -2^2 == -(2^2)
 *
 * This function participates in the recursive relationship between
 * unary and power parsing to implement right-associative exponentiation:
 *
 *     a^b^c == a^(b^c)
 *
 * @return The AST node representing the exponent expression.
 */
ast::NodePtr Parser::parseExponent() { return parseUnary(); }

ast::NodePtr Parser::parseUnary() {
    if (match(TokenType::Plus)) {
        return std::make_unique<ast::UnaryNode>(ast::UnaryOp::Plus,
                                                parseUnary());
    }

    if (match(TokenType::Minus)) {
        return std::make_unique<ast::UnaryNode>(ast::UnaryOp::Minus,
                                                parseUnary());
    }

    return parsePower();
}

/**
 * @brief Parses postfix expressions.
 *
 * Postfix operators have higher precedence than power expressions.
 * Multiple postfix operators may be chained:
 *
 *     3!!! == ((3!)!)!
 *
 * @return The AST node representing the postfix expression.
 */
ast::NodePtr Parser::parsePostfix() {
    auto node = parsePrimary();

    while (match(TokenType::Factorial)) {
        node = std::make_unique<ast::PostfixNode>(
            ast::PostfixOp::Factorial,
            std::move(node));
    }

    return node;
}

/**
 * @brief Parses primary expressions.
 *
 * Primary expressions are the most basic elements of the grammar:
 *
 *     - numeric literals
 *     - identifiers
 *     - function calls
 *     - parenthesized expressions
 *     - absolute value expressions
 *
 * @return The AST node representing the primary expression.
 */
ast::NodePtr Parser::parsePrimary() {
    if (check(TokenType::Number)) {
        return parseNumber();
    }

    if (check(TokenType::Identifier)) {
        return parseIdentifier();
    }

    if (check(TokenType::LParen)) {
        return parseParenExpression();
    }

    if (check(TokenType::VerticalBar)) {
        return parseAbsolute();
    }

    throw std::logic_error("Expected expression.");    
}

//--------------------------------------------------------------
// Primary expressions
//--------------------------------------------------------------

ast::NodePtr Parser::parseNumber() {
    std::string value = current_.lexeme;
    advance();
    return std::make_unique<ast::NumberNode>(std::move(value));
}

ast::NodePtr Parser::parseIdentifier() {
    std::string name = current_.lexeme;
    advance();
    if (!match(TokenType::LParen)) {
        return std::make_unique<ast::IdentifierNode>(std::move(name));
    }

    auto arguments = parseArguments();

    return std::make_unique<ast::FunctionCallNode>(std::move(name),
                                                   std::move(arguments));
}

std::vector<ast::NodePtr> Parser::parseArguments() {
    std::vector<ast::NodePtr> arguments;

    // Empty argument list: f()
    if (match(TokenType::RParen)) {
        return arguments;
    }

    do {
        arguments.push_back(parseExpression());
    } while (match(TokenType::Comma));

    expect(TokenType::RParen, "Expected ')' after function arguments.");

    return arguments;
}

/**
 * @brief Parses a parenthesized expression.
 *
 * Parentheses are used only to control operator precedence and do not
 * generate a dedicated AST node.
 *
 * Examples:
 *
 *     (a + b)
 *     (x^2 + y^2)
 *
 * @return The AST node representing the enclosed expression.
 */
ast::NodePtr Parser::parseParenExpression() {
    expect(TokenType::LParen, "Expected '('.");

    auto expression = parseExpression();

    expect(TokenType::RParen,
           "Expected ')' after expression.");

    return expression;
}

/**
 * @brief Parses absolute value expressions.
 *
 * Absolute value expressions are enclosed by vertical bars:
 *
 *     |x|
 *     |a-b|
 *
 * The vertical bars do not create a precedence level by themselves;
 * they simply group the enclosed expression as an absolute value.
 *
 * @return The AST node representing the absolute value expression.
 */
ast::NodePtr Parser::parseAbsolute() {
    expect(TokenType::VerticalBar, "Expected '|'. ");

    auto expression = parseExpression();

    expect(TokenType::VerticalBar,
           "Expected closing '|'. ");

    return std::make_unique<ast::AbsoluteNode>(
        std::move(expression));
}

}  // namespace numathap::parser
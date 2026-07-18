#include "numathap/parser/Parser.hpp"

#include <memory>
#include <stdexcept>
#include <vector>

#include "numathap/parser/Token.hpp"

namespace numathap::parser {

/**
 * @file Parser.cpp
 * @brief Recursive-descent parser implementation.
 */
Parser::Parser(Lexer& lexer) : lexer_(lexer) { advance(); }

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
 * This is the public entry point of the parser. It validates that the
 * entire input constitutes a single valid expression, ensuring no
 * unexpected tokens remain at the end.
 *
 * @return The root node of the constructed Abstract Syntax Tree (AST).
 * @throws std::logic_error if the expression is invalid or has trailing tokens.
 */
NodePtr Parser::parse() {
    auto expression = parseExpression();

    if (!isAtEnd()) {
        throw std::logic_error("Unexpected token after expression.");
    }

    return expression;
}

//--------------------------------------------------------------
// Grammar
//--------------------------------------------------------------

NodePtr Parser::parseExpression() { return parseAdditive(); }

NodePtr Parser::parseAdditive() {
    auto left = parseMultiplicative();

    while (true) {
        BinaryOp op;

        if (match(TokenType::Plus)) {
            op = BinaryOp::Add;
        } else if (match(TokenType::Minus)) {
            op = BinaryOp::Subtract;
        } else {
            break;
        }

        auto right = parseMultiplicative();

        left =
            std::make_unique<BinaryNode>(op, std::move(left), std::move(right));
    }

    return left;
}

NodePtr Parser::parseMultiplicative() {
    auto left = parseUnary();

    while (true) {
        BinaryOp op;

        if (match(TokenType::Star)) {
            op = BinaryOp::Multiply;
        } else if (match(TokenType::Slash)) {
            op = BinaryOp::Divide;
        } else {
            break;
        }

        auto right = parseUnary();

        left =
            std::make_unique<BinaryNode>(op, std::move(left), std::move(right));
    }

    return left;
}

/**
 * @brief Parses power expressions (right-associative).
 *
 * Handles the '^' operator. Power expressions are right-associative:
 * @code
 * a^b^c == a^(b^c)
 * @endcode
 *
 * The left operand is parsed as a postfix expression to support:
 * @code
 * 3!^2 == (3!)^2
 * @endcode
 *
 * Exponents are parsed via parseExponent() to allow signed values:
 * @code
 * 2^-3 == 2^(-3)
 * @endcode
 *
 * @return AST node for the power expression.
 */
NodePtr Parser::parsePower() {
    auto left = parsePostfix();

    if (!match(TokenType::Caret)) {
        return left;
    }

    auto right = parseExponent();

    return std::make_unique<BinaryNode>(BinaryOp::Power, std::move(left),
                                        std::move(right));
}

/**
 * @brief Parses an exponent.
 *
 * An exponent is treated as a unary expression. This preserves precedence
 * such that unary operators have lower priority than power:
 * @code
 * -2^2 == -(2^2)
 * @endcode
 *
 * @return AST node for the exponent.
 */
NodePtr Parser::parseExponent() { return parseUnary(); }

NodePtr Parser::parseUnary() {
    if (match(TokenType::Plus)) {
        return std::make_unique<UnaryNode>(UnaryOp::Plus, parseUnary());
    }

    if (match(TokenType::Minus)) {
        return std::make_unique<UnaryNode>(UnaryOp::Minus, parseUnary());
    }

    return parsePower();
}

/**
 * @brief Parses postfix expressions (e.g., factorial).
 *
 * Postfix operators have the highest precedence and can be chained:
 * @code
 * 3!!! == ((3!)!)!
 * @endcode
 *
 * @return AST node for the postfix expression.
 */
NodePtr Parser::parsePostfix() {
    auto node = parsePrimary();

    while (match(TokenType::Factorial)) {
        node = std::make_unique<PostfixNode>(PostfixOp::Factorial,
                                             std::move(node));
    }

    return node;
}

/**
 * @brief Parses primary expressions.
 *
 * Dispatches to the appropriate parsing function based on the current token:
 * - Literals (Number)
 * - Identifiers (Variables/Functions)
 * - Grouped expressions (Parentheses)
 * - Absolute values (Vertical bars)
 *
 * @return AST node for the primary expression.
 */
NodePtr Parser::parsePrimary() {
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

NodePtr Parser::parseNumber() {
    std::string value = current_.lexeme;
    advance();
    return std::make_unique<NumberNode>(std::move(value));
}

NodePtr Parser::parseIdentifier() {
    std::string name = current_.lexeme;
    advance();
    if (!match(TokenType::LParen)) {
        return std::make_unique<IdentifierNode>(std::move(name));
    }

    auto arguments = parseArguments();

    return std::make_unique<FunctionCallNode>(std::move(name),
                                              std::move(arguments));
}

std::vector<NodePtr> Parser::parseArguments() {
    std::vector<NodePtr> arguments;

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
 * Parentheses group expressions, overriding precedence rules.
 * They do not produce a dedicated AST node as they are transparent.
 *
 * @return The root node of the enclosed expression.
 */
NodePtr Parser::parseParenExpression() {
    expect(TokenType::LParen, "Expected '('.");

    auto expression = parseExpression();

    expect(TokenType::RParen, "Expected ')' after expression.");

    return expression;
}

/**
 * @brief Parses an absolute value expression: |x|.
 *
 * @return An AbsoluteNode containing the enclosed expression.
 */
NodePtr Parser::parseAbsolute() {
    expect(TokenType::VerticalBar, "Expected '|'. ");

    auto expression = parseExpression();

    expect(TokenType::VerticalBar, "Expected closing '|'. ");

    return std::make_unique<AbsoluteNode>(std::move(expression));
}

}  // namespace numathap::parser
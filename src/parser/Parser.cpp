#include "numathap/parser/Parser.hpp"

#include <memory>
#include <stdexcept>

#include "numathap/ast/Node.hpp"
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

ast::NodePtr Parser::parse() {
    throw std::logic_error("Parser::parse() not implemented.");
}

//--------------------------------------------------------------
// Grammar
//--------------------------------------------------------------

ast::NodePtr Parser::parseExpression() {
    throw std::logic_error("Parser::parseExpression() not implemented.");
}

ast::NodePtr Parser::parseAdditive() {
    throw std::logic_error("Parser::parseAdditive() not implemented.");
}

ast::NodePtr Parser::parseMultiplicative() {
    throw std::logic_error("Parser::parseMultiplicative() not implemented.");
}

ast::NodePtr Parser::parsePower() {
    throw std::logic_error("Parser::parsePower() not implemented.");
}

ast::NodePtr Parser::parseUnary() {
    throw std::logic_error("Parser::parseUnary() not implemented.");
}

ast::NodePtr Parser::parsePostfix() {
    throw std::logic_error("Parser::parsePostfix() not implemented.");
}

ast::NodePtr Parser::parsePrimary() {
    throw std::logic_error("Parser::parsePrimary() not implemented.");
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
    throw std::logic_error("Parser::parseArguments() not implemented.");
}

ast::NodePtr Parser::parseParenExpression() {
    throw std::logic_error("Parser::parseParenExpression() not implemented.");
}

ast::NodePtr Parser::parseAbsolute() {
    throw std::logic_error("Parser::parseAbsolute() not implemented.");
}

}  // namespace numathap::parser
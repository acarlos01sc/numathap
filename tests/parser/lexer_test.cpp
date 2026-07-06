#include "numathap/parser/Lexer.hpp"

#include <iostream>
#include <string>
#include <vector>
#include "numathap/parser/Token.hpp"

using namespace numathap::parser;

std::string TokenToString(TokenType tt) {
    switch (tt) {
        case TokenType::Number:
            return std::string("Number");
        case TokenType::Identifier:
            return std::string("Identifier");
        case TokenType::Plus:
            return std::string("Plus");
        case TokenType::Minus:
            return std::string("Minus");
        case TokenType::Star:
            return std::string("Star");
        case TokenType::Slash:
            return std::string("Slash");
        case TokenType::Caret:
            return std::string("Caret");
        case TokenType::VerticalBar:
            return std::string("VerticalBar");
        case TokenType::RParen:
            return std::string("RParen");
        case TokenType::LParen:
            return std::string("LParen");
        case TokenType::Comma:
            return std::string("Comma");
        case TokenType::Equal:
            return std::string("Equal");
        case TokenType::Less:
            return std::string("Less");
        case TokenType::Greater:
            return std::string("Greater");
        case TokenType::LessEqual:
            return std::string("LessEqual");
        case TokenType::GreaterEqual:
            return std::string("GreaterEqual");
        case TokenType::Factorial:
            return std::string("Factorial");
        case TokenType::NotEqual:
            return std::string("NotEqual");
        case TokenType::Semicolon:
            return std::string("Semicolon");
        case TokenType::EndOfInput:
            return std::string("EndOfInput");
        case TokenType::Invalid:
            return std::string("Invalid");
        default:
            return std::string("Unknow");
    }
}

int main() {
    std::string expr = "sqrt(x^2+y^2)-3.5e-2*sin(theta)+log(1e6)/(2*pi)";

    Lexer lexer(expr);

    std::vector<Token> tokens = lexer.tokenize();
    
    for (const auto& token : tokens) {
        std::cout << TokenToString(token.type) << '\t' << '"' << token.lexeme
                  << '"' << '\t' << '(' << token.line << ',' << token.column
                  << ')' << '\n';
    }

    return 0;
}
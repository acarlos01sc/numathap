#include "numathap/parser/Lexer.hpp"

#include <cctype>
#include <cstddef>
#include <string>

#include "numathap/parser/Token.hpp"

namespace numathap::parser {

Lexer::Lexer(const std::string &input)
    : input_(input), pos_(0), line_(1), column_(1) {}

char Lexer::peek() const {
    if (pos_ >= input_.size()) return '\0';
    return input_[pos_];
}

char Lexer::get() {
    if (pos_ >= input_.size()) return '\0';
    char c = input_[pos_++];

    if (c == '\n') {
        line_++;
        column_ = 1;
    } else {
        column_++;
    }

    return c;
}

void Lexer::skip_white_space() {
    while (std::isspace(peek())) get();
}

Token Lexer::read_number() {
    size_t start = pos_;
    size_t start_line = line_;
    size_t start_column = column_;

    bool has_dot = false;

    while (std::isdigit(peek()) || peek() == '.') {
        if (peek() == '.') {
            if (has_dot) break;
            has_dot = true;
        }
        get();
    }

    return Token{TokenType::Number, input_.substr(start, pos_ - start),
                 start_line, start_column};
}

Token Lexer::read_identifier() {
    size_t start = pos_;
    size_t start_line = line_;
    size_t start_column = column_;

    get();

    while (std::isalnum(peek()) || peek() == '_') get();

    return Token{TokenType::Identifier, input_.substr(start, pos_ - start),
                 start_line, start_column};
}

Token Lexer::next() {
    skip_white_space();

    size_t start_line = line_;
    size_t start_column = column_;

    char c = peek();

    if (c == '\0') {
        return Token{TokenType::EndOfInput, "", start_line, start_column};
    }

    if (std::isdigit(c) || c == '.') return read_number();

    if (std::isalpha(c)) return read_identifier();

    get();

    switch (c) {
        case '+':
            return Token{TokenType::Plus, "+", start_line, start_column};
        case '-':
            return Token{TokenType::Minus, "-", start_line, start_column};
        case '*':
            return Token{TokenType::Star, "*", start_line, start_column};
        case '/':
            return Token{TokenType::Slash, "/", start_line, start_column};
        case '^':
            return Token{TokenType::Caret, "^", start_line, start_column};
        case '(':
            return Token{TokenType::LParen, "(", start_line, start_column};
        case ')':
            return Token{TokenType::RParen, ")", start_line, start_column};
        case '|':
            return Token{TokenType::VerticalBar, "|", start_line, start_column};
        case ',':
            return Token{TokenType::Comma, ",", start_line, start_column};
        default:
            return Token{TokenType::Invalid, std::string(1, c), start_line,
                         start_column};
    }
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        Token t = next();
        tokens.push_back(t);
        if (t.type == TokenType::EndOfInput) break;
    }

    return tokens;
}

}  // namespace numathap::parser
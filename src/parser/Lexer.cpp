#include "numathap/parser/Lexer.hpp"

#include <cctype>
#include <cstddef>
#include <string>

#include "numathap/parser/Token.hpp"




namespace numathap::parser {

Lexer::Lexer(const std::string &input)
    : input_(input), pos_(0), line_(1), column_(1) {}

char Lexer::peek(std::size_t offset) const {
    if (pos_ + offset >= input_.size()) return '\0';
    return input_[pos_ + offset];
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
    while (std::isspace(static_cast<unsigned char>(peek()))) get();
}

Token Lexer::read_number() {
    std::size_t start = pos_;
    std::size_t start_line = line_;
    std::size_t start_column = column_;

    // Integer part
    while (std::isdigit(static_cast<unsigned char>(peek()))) {
        get();
    }

    // Fractional part (optional)
    if (peek() == '.') {
        get();

        while (std::isdigit(static_cast<unsigned char>(peek()))) {
            get();
        }
    }

    // Exponent part (optional)
    // Support scientific notation: 1.2e+10, 1.2e-10, 1.2e10
    if (peek() == 'e' || peek() == 'E') {
        // Check if the next character is a digit or a sign followed by a digit
        bool has_exponent = std::isdigit(static_cast<unsigned char>(peek(1))) ||

                            ((peek(1) == '+' || peek(1) == '-') &&
                             std::isdigit(static_cast<unsigned char>(peek(2))));

        if (has_exponent) {
            get();  // consume 'e' or 'E'

            // Consume optional sign
            if (peek() == '+' || peek() == '-') {
                get();
            }

            while (std::isdigit(static_cast<unsigned char>(peek()))) {
                get();
            }
        }
    }

    return Token{TokenType::Number, input_.substr(start, pos_ - start),
                 start_line, start_column};
}

Token Lexer::read_identifier() {
    std::size_t start = pos_;
    std::size_t start_line = line_;
    std::size_t start_column = column_;

    get();

    while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_')
        get();

    return Token{TokenType::Identifier, input_.substr(start, pos_ - start),
                 start_line, start_column};
}

Token Lexer::next() {
    skip_white_space();

    std::size_t start_line = line_;
    std::size_t start_column = column_;

    char c = peek();

    if (c == '\0') {
        return Token{TokenType::EndOfInput, "", start_line, start_column};
    }

    if (std::isdigit(static_cast<unsigned char>(c)) || c == '.')
        return read_number();

    if (std::isalpha(static_cast<unsigned char>(c))) return read_identifier();

    get();

    switch (c) {
        case '!':
            if (peek() == '=') {
                get();
                return Token{TokenType::NotEqual, "!=", start_line,
                             start_column};
            }
            return Token{TokenType::Factorial, "!", start_line, start_column};
        case '>':
            if (peek() == '=') {
                get();
                return Token{TokenType::GreaterEqual, ">=", start_line,
                             start_column};
            }
            return Token{TokenType::Greater, ">", start_line, start_column};
        case '<':
            if (peek() == '=') {
                get();
                return Token{TokenType::LessEqual, "<=", start_line,
                             start_column};
            }
            return Token{TokenType::Less, "<", start_line, start_column};
        case '=':
            return Token{TokenType::Equal, "=", start_line, start_column};
        case ';':
            return Token{TokenType::Semicolon, ";", start_line, start_column};
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
#pragma once

#include <cstddef>
#include <string>

namespace numathap::parser {
/**
 * @brief Types of lexical tokens recognized by the lexer.
 *
 * This enum represents all possible token categories that can be
 * produced from an input mathematical expression string.
 */
enum class TokenType {
    Number,        ///< Numeric literal (e.g., 3, 3.14, 2e-5)
    Identifier,    ///< Variable or function name (e.g., x, sin, log)
    Plus,          ///< '+' addition operator
    Minus,         ///< '-' subtraction operator
    Star,          ///< '*' multiplication operator
    Slash,         ///< '/' division operator
    Caret,         ///< '^' exponentiation operator
    VerticalBar,   ///< '|' absolute value or delimiter operator
    LParen,        ///< '(' left parenthesis
    RParen,        ///< ')' right parenthesis
    Comma,         ///< ',' function argument separator
    Equal,         ///< '=' equality operator
    Less,          ///< '<' less-than operator
    Greater,       ///< '>' greater-than operator
    LessEqual,     ///< '<=' less-than-or-equal operator
    GreaterEqual,  ///< '>=' greater-than-or-equal operator
    NotEqual,      ///< '!=' not-equal operator
    Semicolon,     ///< ';' expression separator
    EndOfInput,    ///< End of input stream/string
    Invalid        ///< Invalid or unrecognized token
};

/**
 * @brief Represents a lexical token produced by the lexer.
 *
 * A token is the smallest meaningful unit extracted from the input string.
 * It contains its type, the original text (lexeme), and its position in the
 * input.
 */
struct Token {
    /// Token category
    TokenType type;

    /// Original substring matched from input
    std::string lexeme;

    /// Line number where the token starts (1-based indexing)
    std::size_t line;

    /// Column number where the token starts (1-based indexing)
    std::size_t column;
};

}  // namespace numathap::parser
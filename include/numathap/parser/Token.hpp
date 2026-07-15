#pragma once

#include <cstddef>
#include <string>

namespace numathap::parser {

/**
 * @brief Categories of lexical tokens recognized by the lexer.
 *
 * The lexer performs lexical analysis only.
 *
 * It recognizes tokens from the input stream without assigning
 * any mathematical meaning to identifiers, operators or literals.
 *
 * Tokens are produced by the Lexer and consumed by the Parser
 * during syntactic analysis.
 */
/**
 * @note
 * This enumeration describes only lexical categories.
 * The semantic meaning of each token is determined later by the Parser.
 */
enum class TokenType {
    Number,        ///< Numeric literal (e.g., 3, 3.14, 2e-5).
    Identifier,    ///< Variable or function name (e.g., x, sin, log).
    Plus,          ///< Addition operator.
    Minus,         ///< Subtraction operator.
    Star,          ///< Multiplication operator.
    Slash,         ///< Division operator.
    Caret,         ///< Exponentiation operator.
    VerticalBar,   ///< Vertical bar token.
    LParen,        ///< '(' left parenthesis.
    RParen,        ///< ')' right parenthesis.
    Comma,         ///< ',' function argument separator.
    Equal,         ///< '=' equality operator.
    Less,          ///< '<' less-than operator.
    Greater,       ///< '>' greater-than operator.
    LessEqual,     ///< '<=' less-than-or-equal operator.
    GreaterEqual,  ///< '>=' greater-than-or-equal operator.
    Factorial,     ///< '!' factorial operator.
    NotEqual,      ///< '!=' not-equal operator.
    Semicolon,     ///< ';' expression separator.
    EndOfInput,    ///< End of input stream/string.
    Invalid        ///< Unrecognized or malformed token.
};

/**
 * @brief Represents a lexical token produced by the lexer.
 *
 * A token stores:
 * - its lexical category;
 * - the exact lexeme extracted from the input;
 * - the source position (line and column).
 */
struct Token {
    /// Token category
    TokenType type;

    /// Exact substring extracted from the input.
    std::string lexeme;

    /// Line number where the token starts (1-based indexing)
    std::size_t line;

    /// Column number where the token starts (1-based indexing)
    std::size_t column;
};

}  // namespace numathap::parser
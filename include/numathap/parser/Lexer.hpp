/**
 * @file Lexer.hpp
 * @brief Lexical analyzer for mathematical expressions.
 */
#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Token.hpp"

namespace numathap::parser {

/**
 * @brief Lexical analyzer for mathematical expressions.
 *
 * The lexer converts an input string into a sequence of lexical tokens
 * that can be consumed by the parser. It recognizes numeric literals,
 * identifiers, operators, delimiters, and other supported symbols.
 *
 * @note The lexer performs lexical analysis only. It does not validate
 * the syntactic correctness of the expression.
 * @invariant The lexer maintains a valid source position corresponding to the
 * next character to be processed.
 */
class Lexer {
   public:
    /**
     * @brief Constructs a lexer for the given input string.
     * @param input Input expression to tokenize.
     */
    explicit Lexer(const std::string &input);

    /**
     * @brief Returns the next token from the input.
     *
     * Successive calls advance the current position until an
     * EndOfInput token is reached.
     *
     * @return The next lexical token.
     * @see Token
     */
    [[nodiscard]]
    Token next();

    /**
     * @brief Tokenizes the entire input string.
     *
     * Repeatedly invokes next() until an EndOfInput token is produced.
     *
     * @return A vector containing all tokens, including the EndOfInput token.
     */
    [[nodiscard]]
    std::vector<Token> tokenize();

   private:
    char peek(std::size_t offset = 0) const;
    char get();
    void skip_white_space();
    Token read_number();
    Token read_identifier();

    const std::string &input_;  ///< Source expression being tokenized.
    std::size_t pos_;           ///< Current character index.
    std::size_t line_;          ///< Current source line (1-based).
    std::size_t column_;        ///< Current column (1-based).
};

}  // namespace numathap::parser
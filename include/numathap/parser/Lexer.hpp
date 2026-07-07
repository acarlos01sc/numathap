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
 * identifiers, operators, delimiters, and other supported symbols while
 * tracking the position of each token in the input.
 *
 * The lexer performs lexical analysis only. It does not validate the
 * syntactic correctness of the expression.
 */
/**
 * @invariant
 * The lexer maintains a valid source position corresponding to the
 * next character to be processed.
 */
class Lexer {
   public:
    /**
     * @brief Constructs a lexer for the given input string.
     *
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
     */
    [[nodiscard]]
    Token next();

    /**
     * @brief Tokenizes the entire input string.
     *
     * Repeatedly invokes next() until an EndOfInput token is produced.
     *
     * @return A vector containing all tokens, including the
     *         EndOfInput token.
     */
    [[nodiscard]]
    std::vector<Token> tokenize();

   private:
    /**
     * @brief Returns the character located at the specified offset from the
     * current input position without consuming it.
     *
     * @param offset Offset relative to the current position.
     * @return The requested character, or '\0' if the offset is out of
     *         bounds.
     */
    char peek(std::size_t offset = 0) const;

    /**
     * @brief Returns the current character and advances the input position.
     * This function also updates the current line and column counters.
     *
     * @return The consumed character.
     */
    char get();

    /**
     * @brief Consumes consecutive whitespace characters.
     *
     * Updates the current source position while skipping spaces,
     * tabs, and newline characters.
     */
    void skip_white_space();

    /**
     * @brief Reads a numeric literal from the input.
     *
     * Supports integer, floating-point, and scientific notation. The lexical
     * validity of the number is checked during tokenization.
     *
     * @return A Number token.
     */
    Token read_number();

    /**
     * @brief Reads an identifier from the input.
     *
     * Identifiers may represent variables, constants or function names.
     * Their semantic meaning is determined later by the Parser or Evaluator.
     *
     * @return An Identifier token.
     */
    Token read_identifier();

    /// Reference to the source expression being tokenized.
    const std::string &input_;

    /// Current character index within the input string.
    std::size_t pos_;

    /// Current source line (1-based).
    std::size_t line_;

    /// Current column line (1-based).
    std::size_t column_;
};

}  // namespace numathap::parser
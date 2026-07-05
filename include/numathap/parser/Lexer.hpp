#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Token.hpp"

namespace numathap::parser {

class Lexer {
   public:
    explicit Lexer(const std::string &input);
    [[nodiscard]]
    Token next();
    [[nodiscard]]
    std::vector<Token> tokenize();

   private:
    char peek(std::size_t offset = 0) const;
    char get();
    void skip_white_space();

    Token read_number();
    Token read_identifier();

    const std::string &input_;
    std::size_t pos_;
    std::size_t line_;
    std::size_t column_;
};

}  // namespace numathap::parser
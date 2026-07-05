#include <iostream>
#include <string>
#include <vector>

#include "numathap/parser/Lexer.hpp"

using namespace numathap::parser;

int main() {
    std::string expr =
        "sqrt(x^2+y^2)-3.5e-2*sin(theta)+log(1e6)/(2*pi)";

    Lexer lexer(expr);

    auto tokens = lexer.tokenize();

    for (const auto& token : tokens) {
        std::cout
            << static_cast<int>(token.type)
            << '\t'
            << '"' << token.lexeme << '"'
            << '\t'
            << '(' << token.line << ',' << token.column << ')'
            << '\n';
    }

    return 0;
}
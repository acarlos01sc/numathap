#include <fstream>
#include <iostream>
#include <string>

#include "numathap/ast/AstPrinter.hpp"
#include "numathap/parser/Lexer.hpp"
#include "numathap/parser/Parser.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: parser_ast_test <input_file>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file.is_open()) {
        std::cerr << "Could not open file: "
                  << argv[1] << "\n";
        return 1;
    }

    std::string line;
    std::size_t expressionNumber = 1;

    while (std::getline(file, line)) {

        // Ignore empty lines
        if (line.empty()) {
            continue;
        }

        std::cout
            << "========================================\n";
        std::cout
            << "Expression "
            << expressionNumber++
            << ": "
            << line
            << "\n";
        std::cout
            << "----------------------------------------\n";

        try {
            numathap::parser::Lexer lexer(line);

            numathap::parser::Parser parser(lexer);

            auto ast = parser.parse();

            numathap::ast::AstPrinter::print(
                *ast,
                std::cout);
        }
        catch (const std::exception& e) {

            std::cerr
                << "Parser error: "
                << e.what()
                << "\n";
        }

        std::cout << "\n";
    }

    return 0;
}
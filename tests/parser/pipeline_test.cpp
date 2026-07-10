#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "numathap/parser/AstPrinter.hpp"
#include "numathap/parser/Lexer.hpp"
#include "numathap/parser/Parser.hpp"

namespace parser = numathap::parser;

namespace {

/**
 * @brief Returns true if the line contains only whitespace.
 */
bool isBlank(const std::string& line)
{
    return line.find_first_not_of(" \t\r\n") == std::string::npos;
}

/**
 * @brief Processes a single mathematical expression.
 */
void processExpression(const std::string& expression,
                       std::ostream& out,
                       parser::AstPrinter& printer)
{
    out << expression << "\n\n";

    //--------------------------------------------------------------
    // Lexer
    //--------------------------------------------------------------

    parser::Lexer lexer(expression);

    //--------------------------------------------------------------
    // Parser
    //--------------------------------------------------------------

    parser::Parser parser(lexer);

    auto parserAst = parser.parse();

    //--------------------------------------------------------------
    // Parser AST
    //--------------------------------------------------------------

    out << "Parser AST\n";
    out << "----------\n";

    printer.print(out, parserAst);

    //--------------------------------------------------------------
    // Future pipeline
    //
    // MathAstBuilder
    //     ↓
    // Math AST
    //
    // Orchestrator
    //     ↓
    // Prepared AST
    //
    // ExpressionEvaluator
    // Integrator
    // LimitCalculator
    // Differentiator
    //--------------------------------------------------------------
}

} // namespace

int main(int argc, char* argv[])
{
    std::string filename = "parser_ast.txt";

    if (argc > 1)
    {
        filename = argv[1];
    }

    std::ifstream input(filename);

    if (!input)
    {
        std::cerr << "Unable to open file: "
                  << filename
                  << '\n';
        return EXIT_FAILURE;
    }

    std::ostream& out = std::cout;

    parser::AstPrinter printer;

    std::string expression;
    std::size_t lineNumber = 0;

    while (std::getline(input, expression))
    {
        ++lineNumber;

        if (isBlank(expression))
        {
            continue;
        }

        out << "============================================================\n";
        out << "Expression #" << lineNumber << '\n';
        out << "============================================================\n\n";

        try
        {
            processExpression(expression, out, printer);
        }
        catch (const std::exception& ex)
        {
            out << "ERROR\n";
            out << "-----\n";
            out << ex.what() << '\n';
        }

        out << "\n";
    }

    return EXIT_SUCCESS;
}
#include <fstream>
#include <iostream>
#include <string>

#include "numathap/parser/ParserAst.hpp"
#include "numathap/math/MathAst.hpp"

int main()
{
    std::ifstream file("parser_cases.txt");

    if (!file)
    {
        std::cerr << "Cannot open parser_cases.txt\n";
        return 1;
    }

    std::string expression;

    while (std::getline(file, expression))
    {
        if (expression.empty())
            continue;

        std::cout
            << "\n============================================================\n";

        std::cout
            << "Expression\n"
            << "------------------------------------------------------------\n";

        std::cout << expression << "\n\n";

        try
        {
            //------------------------------------------------------
            // Parser AST
            //------------------------------------------------------

            numathap::parser::ParserAst parserAst(expression);

            std::cout
                << "Parser AST\n"
                << "------------------------------------------------------------\n";

            parserAst.print(std::cout);

            //------------------------------------------------------
            // Math AST
            //------------------------------------------------------

            numathap::math::MathAst mathAst(parserAst);

            std::cout
                << "\nMath AST\n"
                << "------------------------------------------------------------\n";

            mathAst.print(std::cout);
        }
        catch (const std::exception& e)
        {
            std::cout << "ERROR: " << e.what() << '\n';
        }
    }

    return 0;
}
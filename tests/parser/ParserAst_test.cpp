#include <fstream>
#include <iostream>
#include <string>

#include "numathap/parser/ParserAst.hpp"

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

        std::cout
            << "Parser AST\n"
            << "------------------------------------------------------------\n";

        try
        {
            numathap::parser::ParserAst ast(expression);

            ast.print(std::cout);
        }
        catch (const std::exception& e)
        {
            std::cout << "ERROR: " << e.what() << '\n';
        }
    }
}
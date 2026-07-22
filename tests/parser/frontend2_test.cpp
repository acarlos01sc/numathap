#include <fstream>
#include <iostream>
#include <string>

#include "numathap/parser/ParserAst.hpp"

#include "numathap/math/MathAst.hpp"
#include "numathap/math/MathAstPrinter.hpp"

#include "numathap/config/configure.hpp"
#include "numathap/config/Capability.hpp"

#include "numathap/orchestration/Orchestrator.hpp"


int main()
{
    std::ifstream file("parser_cases.txt");

    if (!file)
    {
        std::cerr << "Cannot open parser_cases.txt\n";
        return 1;
    }


    //--------------------------------------------------------------
    // Configure environments
    //--------------------------------------------------------------

    auto simplifyEnvironment =
        numathap::config::configure(
            numathap::config::Capability::Simplify
        );


    auto defaultEnvironment =
        numathap::config::configure();



    numathap::orchestration::Orchestrator orchestrator;

    numathap::math::MathAstPrinter printer;


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
            // Math AST before capabilities
            //------------------------------------------------------

            numathap::math::MathAst mathAst(parserAst);


            std::cout
                << "\nMath AST (before capabilities)\n"
                << "------------------------------------------------------------\n";

            mathAst.print(std::cout);



            //------------------------------------------------------
            // With Simplify capability
            //------------------------------------------------------

            std::cout
                << "\nMath AST (with Simplify capability)\n"
                << "------------------------------------------------------------\n";


            auto simplifiedRoot =
                orchestrator.build(
                    mathAst,
                    simplifyEnvironment
                );


            if (simplifiedRoot)
            {
                printer.print(
                    *simplifiedRoot,
                    std::cout
                );
            }
            else
            {
                std::cout
                    << "<empty AST>\n";
            }



            //------------------------------------------------------
            // Without capabilities
            //------------------------------------------------------

            std::cout
                << "\nMath AST (without capabilities)\n"
                << "------------------------------------------------------------\n";


            auto originalRoot =
                orchestrator.build(
                    mathAst,
                    defaultEnvironment
                );


            if (originalRoot)
            {
                printer.print(
                    *originalRoot,
                    std::cout
                );
            }
            else
            {
                std::cout
                    << "<empty AST>\n";
            }
        }
        catch (const std::exception& e)
        {
            std::cout
                << "ERROR: "
                << e.what()
                << '\n';
        }
    }


    return 0;
}
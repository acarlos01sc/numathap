#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "numathap/core/Context.hpp"
#include "numathap/core/Evaluator.hpp"
#include "numathap/parser/Lexer.hpp"
#include "numathap/parser/Parser.hpp"

#ifdef NUMATHAP_PRINT_AST
#include "numathap/ast/AstPrinter.hpp"
#endif

using namespace numathap;

namespace {

constexpr double kTolerance = 1e-12;

std::string trim(std::string s)
{
    const auto first = s.find_first_not_of(" \t\r\n");

    if (first == std::string::npos)
        return "";

    const auto last = s.find_last_not_of(" \t\r\n");

    return s.substr(first, last - first + 1);
}

} // namespace

int main(int argc, char* argv[])
{
    std::string filename = "evaluator.txt";

    if (argc >= 2)
        filename = argv[1];

    std::ifstream input(filename);

    if (!input)
    {
        std::cerr << "Cannot open " << filename << '\n';
        return 1;
    }

    core::Context context;

    std::size_t total = 0;
    std::size_t passed = 0;
    std::size_t lineNumber = 0;

    std::string line;

    while (std::getline(input, line))
    {
        ++lineNumber;

        line = trim(line);

        if (line.empty())
            continue;

        if (line[0] == '#')
            continue;

        auto pos = line.find('=');

        if (pos == std::string::npos)
        {
            std::cerr
                << "Malformed line "
                << lineNumber
                << '\n';

            continue;
        }

        std::string left = trim(line.substr(0, pos));
        std::string right = trim(line.substr(pos + 1));

        //--------------------------------------------------
        // Variable definition
        //--------------------------------------------------

        if (!left.empty() && left.front() == '@')
        {
            std::string name = left.substr(1);
            double value = std::stod(right);

            context.set(name, core::Value(value));

            continue;
        }

        //--------------------------------------------------
        // Expression test
        //--------------------------------------------------

        ++total;

        try
        {
            parser::Lexer lexer(left);
            parser::Parser parser(lexer);

            auto tree = parser.parse();

#ifdef NUMATHAP_PRINT_AST
            std::cout << "\nExpression: " << left << "\n\n";
            ast::AstPrinter printer;
            printer.print(*tree);
            std::cout << '\n';
#endif

            core::Evaluator evaluator(context);

            auto result = evaluator.evaluate(*tree);

            double obtained = result.real().value();
            double expected = std::stod(right);

            if (std::abs(obtained - expected) <= kTolerance)
            {
                ++passed;

                std::cout
                    << "[PASS] "
                    << left
                    << '\n';
            }
            else
            {
                std::cout
                    << "[FAIL] line "
                    << lineNumber
                    << '\n';

                std::cout
                    << "Expression : "
                    << left
                    << '\n';

                std::cout
                    << "Expected   : "
                    << expected
                    << '\n';

                std::cout
                    << "Obtained   : "
                    << obtained
                    << "\n\n";
            }
        }
        catch (const std::exception& e)
        {
            std::cout
                << "[ERROR] line "
                << lineNumber
                << '\n';

            std::cout
                << "Expression : "
                << left
                << '\n';

            std::cout
                << "Message    : "
                << e.what()
                << "\n\n";
        }
    }

    std::cout << "\n----------------------------------------\n";

    std::cout
        << "Passed : "
        << passed
        << '\n';

    std::cout
        << "Failed : "
        << (total - passed)
        << '\n';

    std::cout
        << "Total  : "
        << total
        << '\n';

    std::cout << "----------------------------------------\n";

    return (passed == total) ? 0 : 1;
}
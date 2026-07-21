#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "numathap/math/MathAst.hpp"
#include "numathap/parser/ParserAst.hpp"
#include "numathap/symbolic/Simplifier.hpp"

namespace {

std::string printAst(const numathap::math::MathAst& ast)
{
    std::ostringstream os;
    ast.print(os);
    return os.str();
}

numathap::math::MathAst buildMathAst(
    const std::string& expression)
{
    numathap::parser::ParserAst parserAst(expression);

    return numathap::math::MathAst(parserAst);
}

} // namespace


int main()
{
    using namespace numathap;

    symbolic::Simplifier simplifier;


    const std::vector<std::string> expressions = {
        "x + 1",
        "sin(x)",
        "(x + y) * 2",
        "sin(pi/2)"
    };


    for (const auto& expression : expressions) {

        auto mathAst =
            buildMathAst(expression);


        auto simplified =
            simplifier.simplify(mathAst);


        //
        // The expression source must be preserved.
        //
        assert(
            simplified.expression() == expression);


        //
        // The resulting tree must not be empty.
        //
        assert(
            !simplified.empty());


        //
        // Current Simplifier is an identity transformation.
        //
        assert(
            printAst(simplified) ==
            printAst(mathAst));


        //
        // A new semantic tree must be created.
        //
        assert(
            simplified.root() != mathAst.root());


        std::cout
            << "OK: "
            << expression
            << '\n';
    }


    std::cout
        << "All Simplifier tests passed.\n";

    return 0;
}
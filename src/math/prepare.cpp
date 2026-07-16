#include "numathap/math/prepare.hpp"

#include "numathap/math/MathAst.hpp"


namespace numathap::math {


PreparedAst prepare(
    const std::string& expression,
    const config::MathEnvironment& environment)
{
    parser::ParserAst parserAst(expression);

    MathAst mathAst(parserAst);

    return PreparedAst(mathAst, environment);
}

PreparedAst prepare(const std::string& expression)
{
    config::MathEnvironment environment;
    return prepare(expression, environment);
}

}
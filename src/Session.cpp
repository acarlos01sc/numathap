#include "numathap/Session.hpp"

#include <memory>
#include <utility>

#include "numathap/backend/Evaluator.hpp"
#include "numathap/math/MathAstBuilder.hpp"
#include "numathap/orchestration/Orchestrator.hpp"
#include "numathap/parser/Lexer.hpp"
#include "numathap/parser/Parser.hpp"


namespace numathap {


Session::Session()
    :
    environment_{},
    configurator_{environment_}
{
}


config::Configurator&
Session::configurator() noexcept
{
    return configurator_;
}


const config::Configurator&
Session::configurator() const noexcept
{
    return configurator_;
}


core::Value Session::evaluate(
    const std::string& expression,
    const core::Context& context) const
{
    //----------------------------------------------------------
    // Lexical analysis
    //----------------------------------------------------------

    parser::Lexer lexer(expression);


    //----------------------------------------------------------
    // Parsing
    //----------------------------------------------------------

    parser::Parser parser(lexer);

    auto parserAst =
        parser.parse();


    //----------------------------------------------------------
    // Parser-AST -> Math-AST
    //----------------------------------------------------------

    math::MathAstBuilder builder;

    auto mathAst =
        builder.build(parserAst);


    //----------------------------------------------------------
    // AST preparation
    //----------------------------------------------------------

    orchestration::Orchestrator orchestrator;

    auto preparedAst =
        orchestrator.prepare(
            std::move(mathAst),
            environment_);


    //----------------------------------------------------------
    // Mathematical backend
    //----------------------------------------------------------

    auto adapter =
        environment_.createMathAdapter();


    //----------------------------------------------------------
    // Evaluation
    //----------------------------------------------------------

    backend::Evaluator evaluator(
        context,
        *adapter);


    return evaluator.evaluate(
        *preparedAst);
}


const config::MathEnvironment&
Session::environment() const noexcept
{
    return environment_;
}


} // namespace numathap
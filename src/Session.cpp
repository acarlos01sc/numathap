#include "numathap/Session.hpp"

#include <memory>
#include <utility>

#include "numathap/backend/Evaluator.hpp"
#include "numathap/math/MathAstBuilder.hpp"
#include "numathap/orchestration/Orchestrator.hpp"
#include "numathap/parser/Lexer.hpp"
#include "numathap/parser/Parser.hpp"

namespace numathap {

Session::Session() : environment_{}, configurator_{environment_} {}

//--------------------------------------------------------------
// PreparedExpression
//--------------------------------------------------------------

Session::PreparedExpression::PreparedExpression(
    std::unique_ptr<backend::Evaluator> evaluator)
    : evaluator_(std::move(evaluator)) {}

Session::PreparedExpression::~PreparedExpression() = default;

core::Value Session::PreparedExpression::calc(
    const core::Context& context) const {
    return evaluator_->calc(context);
}

core::Value Session::PreparedExpression::operator()(
    const core::Context& context) const {
    return calc(context);
}

//--------------------------------------------------------------
// Prepare
//--------------------------------------------------------------

Session::PreparedExpression Session::prepare(
    const std::string& expression) const {
    //----------------------------------------------------------
    // Lexical analysis
    //----------------------------------------------------------

    parser::Lexer lexer(expression);

    //----------------------------------------------------------
    // Parsing
    //----------------------------------------------------------

    parser::Parser parser(lexer);

    auto parserAst = parser.parse();

    //----------------------------------------------------------
    // Parser-AST -> Math-AST
    //----------------------------------------------------------

    math::MathAstBuilder builder;

    auto mathAst = builder.build(parserAst);

    //----------------------------------------------------------
    // AST preparation
    //----------------------------------------------------------

    orchestration::Orchestrator orchestrator;

    auto preparedAst = orchestrator.prepare(std::move(mathAst), environment_);

    //----------------------------------------------------------
    // Evaluator
    //----------------------------------------------------------

    auto evaluator =
        std::make_unique<backend::Evaluator>(environment_.mathAdapter());

    evaluator->prepare(std::move(preparedAst));

    return PreparedExpression(std::move(evaluator));
}

//--------------------------------------------------------------
// Evaluate
//--------------------------------------------------------------

core::Value Session::evaluate(const std::string& expression,
                              const core::Context& context) const {
    auto prepared = prepare(expression);

    return prepared.calc(context);
}

//--------------------------------------------------------------
// Configuration
//--------------------------------------------------------------

config::Configurator& Session::configurator() noexcept { return configurator_; }

const config::Configurator& Session::configurator() const noexcept {
    return configurator_;
}

const config::MathEnvironment& Session::environment() const noexcept {
    return environment_;
}

}  // namespace numathap
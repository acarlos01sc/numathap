#include "numathap/parser/ParserAst.hpp"

#include "numathap/parser/Lexer.hpp"
#include "numathap/parser/Parser.hpp"
#include "numathap/parser/ParserAstPrinter.hpp"

namespace numathap::parser {

ParserAst::ParserAst(const std::string& expression) : expression_(expression) {
    Lexer lexer(expression);

    Parser parser(lexer);

    root_ = parser.parse();
}

const std::string& ParserAst::expression() const noexcept {
    return expression_;
}

const Node* ParserAst::root() const noexcept { return root_.get(); }

bool ParserAst::empty() const noexcept { return root_ == nullptr; }

void ParserAst::print(std::ostream& os) const {
    ParserAstPrinter printer;

    printer.print(root_.get(), os);
}

}  // namespace numathap::parser
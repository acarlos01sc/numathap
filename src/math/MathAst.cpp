#include "numathap/math/MathAst.hpp"

#include <utility>

#include "numathap/math/MathAstBuilder.hpp"
#include "numathap/math/MathAstPrinter.hpp"

namespace numathap::math {

MathAst::MathAst(const parser::ParserAst& parserAst)
    : expression_(parserAst.expression()) {
    MathAstBuilder builder;
    root_ = builder.build(parserAst.root());
}

MathAst::MathAst(std::string expression, MathNodePtr root)
    : expression_(std::move(expression)), root_(std::move(root)) {}

const MathNode* MathAst::root() const noexcept { return root_.get(); }

const std::string& MathAst::expression() const noexcept { return expression_; }

bool MathAst::empty() const noexcept { return root_ == nullptr; }

void MathAst::print(std::ostream& os) const {
    if (root_) {
        MathAstPrinter printer;
        printer.print(*root_, os);
    }
}

}  // namespace numathap::math